from fastapi import FastAPI
from pydantic import BaseModel
from typing import List, Optional
import re
from datetime import datetime, timedelta

app = FastAPI(title="Interview Prep Lite API")

INTERVIEW_HINTS = {
    'phone_screen': [r"phone screen", r"phone interview"],
    'behavioral': [r"behavioral", r"culture"],
    'technical_coding': [r"coding interview", r"hackerrank", r"coderpad", r"pair programming"],
    'system_design': [r"system design", r"architecture interview"],
    'oa': [r"online assessment", r"\bOA\b"],
    'manager_round': [r"hiring manager"],
    'onsite_loop': [r"onsite", r"on[- ]site", r"loop", r"panel"]
}

ROLE_REGEX = r"(?:role|position|opening)\s*(?:of|:)?\s*(?P<role>[A-Za-z0-9/ \-]+)"
COMPANY_REGEX = r"(?P<company>[A-Z][A-Za-z0-9& .'\-]+)\s+(?:is|would|we|invites|inviting)"
DATE_REGEXES = [
    r"on\s+(?P<date>\w+\s+\d{1,2},\s+\d{4})\s+at\s+(?P<time>\d{1,2}:\d{2}\s?[AP]M)",
    r"(?P<date>\w+\s+\d{1,2})\s+at\s+(?P<time>\d{1,2}(:\d{2})?\s?[AP]M)"
]

def classify_type(text: str) -> str:
    t = text.lower()
    for label, pats in INTERVIEW_HINTS.items():
        for p in pats:
            if re.search(p, t):
                return label
    return "unknown"

def parse_datetime(text: str) -> Optional[str]:
    for pat in DATE_REGEXES:
        m = re.search(pat, text, flags=re.I)
        if m:
            ds, ts = m.group("date"), m.group("time")
            for fmt in ["%B %d, %Y %I:%M %p", "%B %d %I:%M %p"]:
                try:
                    dt = datetime.strptime(f"{ds} {ts}", fmt)
                    if "%Y" not in fmt:
                        dt = dt.replace(year=datetime.now().year)
                    return dt.isoformat()
                except Exception:
                    pass
    return None

class ParseReq(BaseModel):
    text: str

class ParseResp(BaseModel):
    company: Optional[str] = None
    role: Optional[str] = None
    type: str = "unknown"
    when_iso: Optional[str] = None

@app.post("/parse_email", response_model=ParseResp)
def parse_email(req: ParseReq):
    t = req.text.strip()
    company = None
    m = re.search(COMPANY_REGEX, t)
    if m:
        company = m.group("company").strip().strip(".:,")
    role = None
    m = re.search(ROLE_REGEX, t, flags=re.I)
    if m:
        role = m.group("role").strip().strip(".:,")
    itype = classify_type(t)
    when_iso = parse_datetime(t)
    return ParseResp(company=company, role=role, type=itype, when_iso=when_iso)

class PrepReq(BaseModel):
    company: Optional[str] = None
    role: Optional[str] = None
    type: str = "unknown"

class PrepOut(BaseModel):
    summary: str
    questions: List[str]

TEMPLATES = {
    "behavioral": [
        "Tell me about yourself and why you're interested in {company}.",
        "Describe a time you handled a conflict on a team.",
        "Walk me through a difficult bug you fixed and how you approached it.",
        "Tell me about a time you had to quickly learn a new technology."
    ],
    "technical_coding": [
        "Reverse a linked list; discuss time/space complexity.",
        "Design an LRU cache and write core methods.",
        "Implement binary search on a sorted array; explain invariants.",
        "Two-sum: hash map vs two-pointer trade-offs."
    ],
    "system_design": [
        "Design a URL shortener: API, data model, scaling, availability.",
        "Design a rate limiter for an API gateway (distributed concerns).",
        "Design notifications for a chat app (push vs pull, fan-out)."
    ],
    "phone_screen": [
        "What projects are you most proud of and why?",
        "Describe a challenging technical problem you solved recently.",
        "How do you approach learning unfamiliar tech quickly?"
    ],
    "oa": [
        "Practice arrays/strings (sliding window, two-pointer, prefix sums).",
        "Practice hash map + stack basics and time complexity explanations."
    ],
    "unknown": [
        "Prepare 3 STAR stories from your projects.",
        "Review DS&A fundamentals and one end-to-end system you built."
    ]
}

@app.post("/generate_prep", response_model=PrepOut)
def generate_prep(req: PrepReq):
    itype = req.type if req.type in TEMPLATES else "unknown"
    qs = [q.format(company=req.company or "the company") for q in TEMPLATES[itype]][:6]
    summary = f"Focus on {itype.replace('_',' ')} for '{req.role or 'the role'}' at {req.company or 'the company'}."
    return PrepOut(summary=summary, questions=qs)

class ScheduleReq(BaseModel):
    when_iso: Optional[str] = None
    blocks: int = 3
    minutes_each: int = 60

class ScheduleOut(BaseModel):
    suggestions: List[str]

@app.post("/schedule_suggestions", response_model=ScheduleOut)
def schedule_suggestions(req: ScheduleReq):
    now = datetime.now()
    start_from = now + timedelta(hours=2)
    if req.when_iso:
        try:
            interview_time = datetime.fromisoformat(req.when_iso)
            start_from = min(start_from, interview_time - timedelta(days=3))
        except Exception:
            pass
    slots, cursor = [], start_from.replace(minute=0, second=0, microsecond=0)
    for _ in range(req.blocks):
        slots.append((cursor, cursor + timedelta(minutes=req.minutes_each)))
        cursor += timedelta(hours=6)
    iso = [f"{s.isoformat()} -> {e.isoformat()}" for s, e in slots]
    return ScheduleOut(suggestions=iso)

@app.get("/health")
def health():
    return {"status": "ok"}
