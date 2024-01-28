import express from "express";
import mongoose from "mongoose";
import bodyParser from "body-parser";
import MapRouter from "./routes/maps";
import TextureRouter from "./routes/textures";
import TileRouter from "./routes/tiles";
import SpriteRouter from "./routes/sprites";
import {param} from "express-validator";
import cors from "cors";

const app = express();

app.use(bodyParser.urlencoded({
    extended: false,
    limit:'100mb',
    parameterLimit:1000000
}));
app.use(bodyParser.json({ limit: "50mb" }));
app.use(cors());

app.use("/api/maps/public", express.static("public"));
app.use("/api/maps", MapRouter);
app.use(
    "/api/maps/:mapId/textures", 
    param("mapId").isMongoId(),
    TextureRouter
);
app.use(
    "/api/maps/:mapId/tiles",
    param("mapId").isMongoId(),
    TileRouter
);
app.use(
    "/api/maps/:mapId/sprites",
    param("mapId").isMongoId(),
    SpriteRouter
);

mongoose.connect(process.env.MONGO_URL!!)
    .then(() => console.log("Connected to MongoDB"));

const port = process.env.PORT || 3001;
app.listen(port, () => {
    console.log("Server running on port " + port);
});