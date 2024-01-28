import express, {Request, Response} from "express";
import {body, matchedData, param} from "express-validator";
import Map from "../models/map";
import {validateRequest} from "../utils";

const router = express.Router();

router.get(
    "/",
    validateRequest,
    async (req: Request, res: Response) => {
        const data = matchedData(req);

        try {
            const map = await Map.findById(data.mapId);
            if (!map)
                return res.status(404).send("Map not found");

            return res.send(map.sprites);
        } catch (err) {
            return res
                .status(404)
                .send("Map not found");
        }
    });

router.post(
    "/",
    body("name").notEmpty(),
    body("texture").isMongoId(),
    body("x").isNumeric(),
    body("y").isNumeric(),
    body("width").isNumeric(),
    body("height").isNumeric(),
    validateRequest,
    async (req: Request, res: Response) => {
        const data = matchedData(req);

        try {
            const map = await Map.findById(data.mapId);
            if (!map)
                return res.status(404).send("Map not found");

            map.sprites.push({
                name: data.name,
                x: data.x,
                y: data.y,
                width: data.width,
                height: data.height,
                texture: data.texture
            });
            await map.save();

            return res.send(map.sprites);
        } catch (err) {
            return res
                .status(404)
                .send("Map not found");
        }
    });

router.put(
    "/:spriteId",
    param("spriteId").isMongoId(),
    body("name").notEmpty(),
    body("texture").isMongoId(),
    body("x").isNumeric(),
    body("y").isNumeric(),
    body("width").isNumeric(),
    body("height").isNumeric(),
    validateRequest,
    async (req: Request, res: Response) => {
        const data = matchedData(req);

        try {
            const map = await Map.findById(data.mapId);
            if (!map)
                return res.status(404).send("Map not found");

            const sprite = map.sprites.id(data.spriteId);
            if (!sprite)
                return res.status(404).send("Sprite not found");

            sprite.name = data.name;
            sprite.x = data.x;
            sprite.y = data.y;
            sprite.width = data.width;
            sprite.height = data.height;
            sprite.texture = data.texture;
            await map.save();

            return res.send(map.sprites);
        } catch (err) {
            console.log(err);
            return res
                .status(404)
                .send("Map not found");
        }
    })

router.delete(
    "/:spriteId",
    param("spriteId").isMongoId(),
    validateRequest,
    async (req: Request, res: Response) => {
        const data = matchedData(req);

        try {
            const map = await Map.findById(data.mapId);
            if (!map)
                return res.status(404).send("Map not found");

            map.sprites.remove(data.spriteId);
            await map.save();

            return res.send(map.sprites);
        } catch (err) {
            return res
                .status(404)
                .send("Map not found");
        }
    });

export default router;