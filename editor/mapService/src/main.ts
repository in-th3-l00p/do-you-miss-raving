import express from "express";
import mongoose from "mongoose";
import bodyParser from "body-parser";
import MapRouter from "./routes/maps";
import TextureRouter from "./routes/textures";
import TileRouter from "./routes/tiles";
import { param } from "express-validator";

const app = express();

app.use(bodyParser.json({ limit: "50mb" }));
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
)

mongoose.connect(process.env.MONGO_URL!!)
    .then(r => console.log("Connected to MongoDB"));

const port = process.env.PORT || 3001;
app.listen(port, () => {
    console.log("Server running on port " + port);
});