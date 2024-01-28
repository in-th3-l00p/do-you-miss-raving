<?php

namespace App\Http\Controllers;

use App\Http\Resources\PublicUserResource;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Gate;

class UserController extends Controller {
    public function current() {
        Gate::authorize("authenticated");
        return new PublicUserResource(auth()->user());
    }
}
