<?php

use App\Http\Controllers\UserController;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;

Route::prefix("/auth")->group(function () {
    require "auth.php";

    Route::middleware("auth:sanctum")->group(function () {
        Route::get("/", [UserController::class, "current"])->name("users.current");
    });
});
