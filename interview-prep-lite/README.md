# Interview Prep Lite

Paste an interview email into a simple web UI. The FastAPI backend parses company/role/interview type, generates a short prep set, and suggests study blocks. No OAuth, no database, runs locally.

## Run
### Backend
```bash
cd server
pip install -r requirements.txt
uvicorn app:app --reload --port 8000
```

### Frontend
Open `web/index.html` in your browser. It calls `http://localhost:8000`.

## Files
- server/app.py — FastAPI endpoints: /parse_email, /generate_prep, /schedule_suggestions
- server/requirements.txt — FastAPI deps
- web/index.html — static UI

## License
MIT (for portfolio use)
