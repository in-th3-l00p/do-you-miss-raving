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

export default router;