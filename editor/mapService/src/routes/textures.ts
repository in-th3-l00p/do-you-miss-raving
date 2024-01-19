import express from 'express';
import Map from '../models/map';
import { body, matchedData, validationResult } from 'express-validator';

const router = express.Router();

router.post(
    "/",
    body("name").notEmpty().isString().trim().isLength({ min: 3, max: 255 }),
    body("image").notEmpty().isString().trim(),
    async (req: express.Request, res: express.Response) => {
        const data = matchedData(req);
        let map = await Map.findById(data.mapId);
        if (!map)
            return res.status(404).json({ error: "Map not found" });
        const errors = validationResult(req);
        if (!errors.isEmpty())
            return res
                .status(400)
                .json({ errors });
        
        map!.textures.push(data);
        res.send(await map!.save());
    }
);

export default router;