export interface MongoEntity {
    _id: string;
}

export interface Texture extends MongoEntity {
    label: string;
    path: string;
}

export interface Tile extends MongoEntity {
    texture?: string;
    floor?: string;
    ceiling?: string;
    empty: boolean;
}

export interface Sprite extends MongoEntity {
    name: string;
    texture: string;
    x: number;
    y: number;
    width: number;
    height: number;
    action: string;
}

export interface Map extends MongoEntity {
    name: string;
    width: number;
    height: number;
    playerStartX: number;
    playerStartY: number;
    playerStartDirectionX: number;
    playerStartDirectionY: number;
    enemyStartX?: number;
    enemyStartY?: number;
    image?: string;
    textures?: Texture[];
    tiles?: Tile[];
    sprites?: Sprite[];
}