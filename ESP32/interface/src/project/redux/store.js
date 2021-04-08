import {configureStore} from "@reduxjs/toolkit";
import rootReducer from "./rootReducer";
import reduxWebsocket from '@giantmachines/redux-websocket';
import webSocketMiddleware from "./middleware/webSocketMiddleware";

const reduxWebsocketMiddleware = reduxWebsocket({prefix: "ENCLOSURE_CONTROLLER"});

const store = configureStore({reducer: rootReducer, middleware: [reduxWebsocketMiddleware, webSocketMiddleware]});

export default store;