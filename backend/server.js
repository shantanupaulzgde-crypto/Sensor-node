const express = require('express');
const cors = require('cors');
const app = express();
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());
app.use(express.static('../frontend/public'));

// In-memory sensor data store
let sensorData = {
  temp: 0, hum: 0, mq4: 0, mq7: 0,
  co2: 0, dist: 0, flow: 0, metal: '--',
  timestamp: new Date().toISOString()
};

// History: last 50 readings
let history = [];

// POST endpoint: ESP8266 sends data here
app.post('/api/data', (req, res) => {
  const { temp, hum, mq4, mq7, co2, dist, flow, metal } = req.body;

  sensorData = {
    temp: parseFloat(temp) || 0,
    hum: parseFloat(hum) || 0,
    mq4: parseFloat(mq4) || 0,
    mq7: parseFloat(mq7) || 0,
    co2: parseFloat(co2) || 0,
    dist: parseFloat(dist) || 0,
    flow: parseFloat(flow) || 0,
    metal: metal || '--',
    timestamp: new Date().toISOString()
  };

  history.push({ ...sensorData });
  if (history.length > 50) history.shift();

  console.log(`[${new Date().toLocaleTimeString()}] Data received:`, sensorData);
  res.json({ status: 'ok', received: sensorData });
});

// GET endpoint: frontend fetches latest data
app.get('/api/data', (req, res) => {
  res.json(sensorData);
});

// GET endpoint: history
app.get('/api/history', (req, res) => {
  res.json(history);
});

// Health check
app.get('/api/health', (req, res) => {
  res.json({ status: 'online', uptime: process.uptime() });
});

app.listen(PORT, () => {
  console.log(`✅ Sensor backend running on port ${PORT}`);
});

app.listen(PORT, () => {
  console.log(`✅ Sensor backend running on port ${PORT}`);
});

// ── Keep Render alive (prevents sleep on free tier) ──────────
const https = require('https');
const SELF_URL = 'https://sensor-node.onrender.com/api/health';

setInterval(() => {
  https.get(SELF_URL, (res) => {
    console.log(`[KEEP-ALIVE] Pinged self — status ${res.statusCode}`);
  }).on('error', (e) => {
    console.log('[KEEP-ALIVE] Ping failed:', e.message);
  });
}, 10 * 60 * 1000);
