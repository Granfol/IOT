import * as http from "http";
import express from "express";
import ip from "ip";
import path, { join, dirname } from "path";
import { fileURLToPath } from "url";
import WebSocket, { WebSocketServer } from "ws";

const __dirname = path.dirname(fileURLToPath(import.meta.url));

const app = express();
const server = http.createServer(app);

const hostname = "192.168.115.195";
const port = 80;

const wss1 = new WebSocketServer({ port: 8888 });
const wss2 = new WebSocketServer({ port: 8811 });

app.get("/", function (req, res) {
  res.sendFile(path.join(__dirname, "test16copy.html"));
});

wss1.on("connection", function (ws, req) {
  console.log("Client - Chrome Browser connected via bidirectional connection on port 8888");

  ws.on("message", function (msgBuffer) {
    const msgString = msgBuffer.toString();
    console.log("Message received, content: " + msgString);

    var msg = JSON.parse(msgString);
    switch (msg.tipSporočila) {
      case "LED":
        funkcijaLED(msg);
        break;
    }
  });
});

wss2.on("connection", function (ws, req) {
  console.log("ESP32 connected via bidirectional connection on port 8811");
  ws.on("message", (message) => {
    const messageString = message.toString();
    console.log("Received message:", messageString);
  });
});

wss2.broadcast = function broadcast(data) {
  wss2.clients.forEach(function each(client) {
    if (client.readyState === WebSocket.OPEN) {
      client.send(data);
    }
  });
};

function funkcijaLED(msg) {
  wss2.broadcast(JSON.stringify(msg));
}

server.listen(port, hostname, () => {
  console.log("Server running");
  console.log("IP=" + ip.address());
});
