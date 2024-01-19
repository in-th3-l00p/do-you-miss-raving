import mongoose = require("mongoose");

enum SpriteAction {
    STATIC,
    ENEMY
}

const mapSchema = new mongoose.Schema({
    name: {
        type: String,
        required: true
    },
    width: {
        type: Number,
        min: 1,
        max: 100,
        required: true
    },
    height: {
        type: Number,
        min: 1,
        max: 100,
        required: true
    },
    image: {
        type: String,
        required: false
    },
    textures: [{
        name: String,
        image: Buffer
    }],
    tiles: [{
        texture: {
            type: String,
            ref: "Texture",
            required: false
        },
        floor: {
            type: String,
            ref: "Texture",
            required: false
        },
        ceiling: {
            type: String,
            ref: "Texture",
            required: false
        },
        empty: {
            type: Boolean,
            required: true
        }
    }],
    sprites: [{
        texture: {
            type: String,
            ref: "Texture",
            required: true
        },
        x: {
            type: Number,
            required: true
        },
        y: {
            type: Number,
            required: true
        },
        width: {
            type: Number,
            required: true
        },
        height: {
            type: Number,
            required: true
        },
        action: {
            type: String,
            enum: SpriteAction,
            required: true
        }
    }]
});

const Map = mongoose.model("Map", mapSchema);
export default Map;
