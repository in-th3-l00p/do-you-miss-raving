import express from "express";
import { validationResult, matchedData, body, query } from "express-validator";
import Map from "../models/map";
import { Request, Response } from "express";

const router = express.Router();

router.get("/", async (req: any, res: any) => {
    const errors = validationResult(req);
    if (!errors.isEmpty())
        return res
            .status(400)
            .json(errors);
    const { mapId } = matchedData(req);
    const textures = await Map.findById(mapId).select("textures");
    if (!textures)
        return res.status(404).json({ error: "Map not found" });
    return textures;
});

router.put(
    "/",
    query("x").notEmpty().isInt({ min: 0, max: 100 }),
    query("y").notEmpty().isInt({ min: 0, max: 100 }),
    body("texture").isMongoId(),
    body("floor").optional().isMongoId(),
    body("ceiling").optional().isMongoId(),
    body("empty").notEmpty().isBoolean(),
    async (req: Request, res: Response) => {
        const errors = validationResult(req);
        if (!errors.isEmpty())
            return res
                .status(400)
                .json(errors);
        const data = matchedData(req);
        const map = await Map.findById(data.mapId);
        if (!map)
            return res
                    .status(404)
                    .json({ error: "Map not found" });
        if (data.x >= map.width || data.y >= map.height || data.x < 0 || data.y < 0)
            return res
                    .status(400)
                    .json({ error: "Invalid coordinates" });
        console.log(data.y * map.width + data.x); // ce pula mea
        map.tiles[data.y * map.width + data.x].texture = data.texture;
        map.tiles[data.y * map.width + data.x].floor = data.floor;
        map.tiles[data.y * map.width + data.x].ceiling = data.ceiling;
        map.tiles[data.y * map.width + data.x].empty = data.empty;
        await map.save();

        res.send(await Map
            .findById(map.id)
            .select("-image -textures -tiles -sprites"));
    }
)

export default router;