import express from "express";
import { validationResult, matchedData, body, query } from "express-validator";
import Map from "../models/map";
import { Request, Response } from "express";

const router = express.Router();

router.get("/", async (req: Request, res: Response) => {
    const errors = validationResult(req);
    if (!errors.isEmpty())
        return res
            .status(400)
            .json(errors);
    const { mapId } = matchedData(req);
    const textures = await Map.findById(mapId).select("textures");
    if (!textures)
        return res.status(404).json({ errors: [
                {
                    "type": "notFound",
                    "msg": "Map not found"
                }
            ]});
    res.send(textures);
});

router.put(
    "/",
    query("x").notEmpty().isInt({ min: 0, max: 100 }),
    query("y").notEmpty().isInt({ min: 0, max: 100 }),
    body("texture").optional().isMongoId(),
    body("floor").optional().isMongoId(),
    body("ceiling").optional().isMongoId(),
    body("empty").notEmpty().isBoolean(),
    async (req: Request, res: Response) => {
        const errors = validationResult(req);
        if (!errors.isEmpty())
            return res
                .status(400)
                .json(errors);
        let data = matchedData(req);
        data.x = parseInt(data.x);
        data.y = parseInt(data.y);

        let map;
        try {
            map = await Map.findById(data.mapId);
        } catch (err) {
            return res.status(404).json({
                errors: [
                    {
                        "type": "notFound",
                        "msg": "Map not found"
                    }
                ]
            });
        }
        if (!map)
            return res.status(404).json({
                errors: [
                    {
                        "type": "notFound",
                        "msg": "Map not found"
                    }
                ]
            });

        if (data.x >= map.width || data.y >= map.height || data.x < 0 || data.y < 0)
            return res.status(404).json({ errors: [
                    {
                        "type": "notFound",
                        "msg": "Map not found"
                    }
                ]});

        if (data.texture && map.textures.find(t => t.id === data.texture))
            map.tiles[data.y * map.width + data.x].texture = data.texture;
        else if (data.texture === "")
            map.tiles[data.y * map.width + data.x].texture = null;

        if (data.floor && map.textures.find(t => t.id === data.floor))
            map.tiles[data.y * map.width + data.x].floor = data.floor;
        else if (data.floor === "")
            map.tiles[data.y * map.width + data.x].floor = null;

        if (data.ceiling && map.textures.find(t => t.id === data.ceiling))
            map.tiles[data.y * map.width + data.x].ceiling = data.ceiling;
        else if (data.ceiling === "")
            map.tiles[data.y * map.width + data.x].ceiling = null;

        map.tiles[data.y * map.width + data.x].empty = data.empty;
        await map.save();

        res.send(await Map
            .findById(map.id)
            .select("-image -textures -sprites"));
    }
);

export default router;