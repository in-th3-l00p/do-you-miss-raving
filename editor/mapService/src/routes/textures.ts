import express, {Request, Response} from 'express';
import Map from '../models/map';
import {body, matchedData, param, validationResult} from 'express-validator';
import multer from "multer";
import {validateRequest} from "../utils";
import fs from "fs";
import path from "path";

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

router.put(
    "/:textureId",
    upload.single("image"),
    param("textureId").isMongoId(),
    body("label")
        .notEmpty()
        .isString()
        .trim()
        .isLength({ min: 3, max: 255 }),
    validateRequest,
    async (req: MulterRequest, res: Response) => {
        const data = matchedData(req);
        try {
            const map = await Map.findOne({
                "_id": data.mapId
            });
            if (!map)
                return res.status(404).json({error: "Map not found"});

            for (let texture of map.textures) {
                if (texture._id == data.textureId) {
                    texture.label = data.label;
                    if (req.file) {
                        fs.rm(path.join(process.cwd(), texture.path!), () => {});
                        texture.filetype = req.file.mimetype;
                        texture.path = req.file.path;
                    }

                    await map.save();
                }
            }

            res.send(map.textures);
        } catch (err) {
            res.status(404).json({error: "Map not found"});
        }
    });

export default router;