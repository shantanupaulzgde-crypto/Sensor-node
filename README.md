# Smart Sensor Dashboard — Full Stack

## Project Structure

```
sensor-dashboard/
├── backend/
│   ├── server.js          # Express API server
│   └── package.json
├── frontend/
│   └── public/
│       └── index.html     # Full dashboard UI
├── esp8266_sender/
│   └── esp8266_sender.ino # Updated ESP8266 Arduino code
└── README.md
```

---

## 🚀 Deploy Backend Online (Free) — Render.com

1. Create account at https://render.com
2. Click **New → Web Service**
3. Connect your GitHub repo (upload the `backend/` folder)
4. Set:
   - **Root Directory**: `backend`
   - **Build Command**: `npm install`
   - **Start Command**: `node server.js`
   - **Environment**: Node
5. Click **Create Web Service**
6. You'll get a URL like: `https://your-app.onrender.com`

---

## 🔌 API Endpoints

| Method | Endpoint      | Description                        |
|--------|---------------|------------------------------------|
| GET    | /api/data     | Get latest sensor readings         |
| POST   | /api/data     | Send new sensor data (from ESP)    |
| GET    | /api/history  | Get last 50 readings               |
| GET    | /api/health   | Server health check                |

### POST /api/data — JSON Body
```json
{
  "temp": 25.4,
  "hum": 60.2,
  "mq4": 312,
  "mq7": 198,
  "co2": 420,
  "dist": 14.5,
  "flow": 1.23,
  "metal": "NONE"
}
```
`metal` should be either `"DETECTED"` or `"NONE"`.

---

## 🌐 Access the Dashboard

Open `frontend/public/index.html` in a browser, or host it on:
- **GitHub Pages** — push to a repo, enable Pages from Settings
- **Netlify** — drag and drop the `frontend/public/` folder
- **Vercel** — import the repo

On the dashboard, enter your Render backend URL in the config box at the bottom.

---

## 🔧 ESP8266 Setup

1. Open `esp8266_sender/esp8266_sender.ino` in Arduino IDE
2. Replace `YOUR_BACKEND_URL` with your Render URL
3. Update `ssid` and `password`
4. Add your sensor reading code in `loop()`
5. Install required libraries:
   - **ArduinoJson** by Benoit Blanchon (Library Manager)
6. Upload to ESP8266

---

## Local Testing

```bash
cd backend
npm install
node server.js
# Server runs at http://localhost:3000
```

Test with curl:
```bash
curl -X POST http://localhost:3000/api/data \
  -H "Content-Type: application/json" \
  -d '{"temp":25.5,"hum":60,"mq4":300,"mq7":200,"co2":415,"dist":12,"flow":1.1,"metal":"NONE"}'
```
