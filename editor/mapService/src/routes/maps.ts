import express from 'express';
import { body, validationResult, matchedData } from 'express-validator';
import Map from '../models/map';

const router = express.Router();

router.post(
    "/",
    body("name").notEmpty().trim().isLength({ min: 3, max: 255 }),
    body("width").notEmpty().isInt({ min: 1, max: 100 }),
    body("height").notEmpty().isInt({ min: 1, max: 100 }),
    async (req: express.Request, res: express.Response) => {
        const errors = validationResult(req);
        if (!errors.isEmpty())
            return res
                .status(400)
                .json(errors);
        const data = matchedData(req);
        const { _id } = await Map.create({
            ...data,
            tiles: Array
                    .from({ length: data.width * data.height })
                    .map(() => ({ empty: true }))
        });
        res.send(await Map
            .findById(_id)
            .select("-image -textures -tiles -sprites"));
    });

router.get("/", async (req: any, res: any) => {
    res.send(await Map
        .find()
        .select("-image -textures -tiles -sprites"));
});

router.get("/:id", async (req: any, res: any) => {
    try {
        const map = await Map.findById(req.params.id);
        res.send(map);
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
});

router.put(
    "/:id/playerCoord",
    body("x").notEmpty().isInt({ min: 0 }),
    body("y").notEmpty().isInt({ min: 0 }),
    async (req: any, res: any) => {
        const errors = validationResult(req);
        if (!errors.isEmpty())
            return res
                .status(400)
                .json(errors);
        const data = matchedData(req);
        try {
            const map = await Map.findById(req.params.id);
            if (!map)
                throw new Error("Map not found");
            map.playerStartX = data.x;
            map.playerStartY = data.y;
            await map.save();
            res.send(map);
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
    });

router.put(
    "/:id/enemyCoord",
    body("x").notEmpty().isInt({ min: 0 }),
    body("y").notEmpty().isInt({ min: 0 }),
    async (req: any, res: any) => {
        const errors = validationResult(req);
        if (!errors.isEmpty())
            return res
                .status(400)
                .json(errors);
        const data = matchedData(req);
        try {
            const map = await Map.findById(req.params.id);
            if (!map)
                throw new Error("Map not found");
            map.enemyStartX = data.x;
            map.enemyStartY = data.y;
            await map.save();
            res.send(map);
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
    });

router.delete("/:id/enemyCoord", async (req: any, res: any) => {
    try {
        const map = await Map.findById(req.params.id);
        if (!map)
            throw new Error("Map not found");
        map.enemyStartX = undefined;
        map.enemyStartY = undefined;
        await map.save();
        res.send(map);
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
});

router.put(
    "/:id/playerDirection",
    body("x").notEmpty().isFloat({ min: 0, max: 1 }),
    body("y").notEmpty().isFloat({ min: 0, max: 1 }),
    async (req: any, res: any) => {
        const errors = validationResult(req);
        if (!errors.isEmpty())
            return res
                .status(400)
                .json(errors);
        const data = matchedData(req);
        try {
            const map = await Map.findById(req.params.id);
            if (!map)
                throw new Error("Map not found");
            map.playerStartDirectionX = data.x;
            map.playerStartDirectionY = data.y;
            await map.save();
            res.send(map);
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
    });

export default router;