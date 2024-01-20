import express, {Request, Response} from 'express';
import Map from '../models/map';
import {body, matchedData, validationResult} from 'express-validator';
import multer from "multer";

const router = express.Router();

const upload = multer({ dest: "public/textures/" });
interface MulterRequest extends Request {
    file?: any;
}

router.post(
    "/",
    upload.single("image"),
    body("label")
        .notEmpty()
        .isString()
        .trim()
        .isLength({ min: 3, max: 255 }),
    async (req: MulterRequest, res: Response) => {
        const data = matchedData(req);
        let map;
        try {
            map = await Map.findById(data.mapId);
            if (!map)
                return res.status(404).json({error: "Map not found"});
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
        const errors = validationResult(req);
        if (!errors.isEmpty())
            return res
                .status(400)
                .json(errors);

        try {
            map!.textures.push({
                label: data.label,
                filetype: req.file!.mimetype,
                path: req.file!.path
            });
        } catch (err) {
            return res.status(500).json({ error: "Internal server error" });
        }
        map = await map!.save();

        const texture = map!.textures[map!.textures.length - 1];
        res.send(texture);
    }
);

export default router;