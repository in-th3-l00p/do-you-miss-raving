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
    playerStartX: {
        type: Number,
        min: 0,
        required: true,
        default: 0
    },
    playerStartY: {
        type: Number,
        min: 0,
        required: true,
        default: 0
    },
    playerStartDirectionX: {
        type: Number,
        min: 0,
        max: 1,
        required: true,
        default: 1
    },
    playerStartDirectionY: {
        type: Number,
        min: 0,
        max: 1,
        required: true,
        default: 0
    },
    enemyStartX: {
        type: Number,
        min: 0,
        required: false
    },
    enemyStartY: {
        type: Number,
        min: 0,
        required: false
    },
    image: {
        type: String,
        required: false
    },
    textures: [{
        label: String,
        filetype: String,
        path: String
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
