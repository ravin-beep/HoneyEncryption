# Honey Encryption Full-Stack System

A full-stack implementation of Honey Encryption with Fake Plaintext Generation. This system demonstrates deception-based cryptography where every decryption attempt produces meaningful output.

## Architecture

```
Frontend (React + TailwindCSS)
        ↓
REST API (FastAPI)
        ↓
Honey Encryption Module (Python)
        ↓
PostgreSQL Encrypted Storage
```

## Prerequisites

- Python 3.9+
- Node.js 18+
- PostgreSQL 14+

## Quick Start

### 1. Database Setup

```bash
# Create PostgreSQL database
createdb honey_encryption

# Run the schema (or let the app create it automatically)
psql -d honey_encryption -f database/schema.sql
```

### 2. Backend Setup

```bash
cd backend

# Create virtual environment
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt

# Start the server
python main.py
```

The backend will run on http://localhost:8000

### 3. Frontend Setup

```bash
cd frontend

# Install dependencies
npm install

# Start development server
npm run dev
```

The frontend will run on http://localhost:3000

## Environment Variables

Create a `.env` file in the `backend` directory:

```env
DATABASE_URL=postgresql://postgres:yourpassword@localhost:5432/honey_encryption
HOST=0.0.0.0
PORT=8000
DEBUG=true
```

## API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/encrypt` | POST | Encrypt a message |
| `/api/decrypt` | POST | Decrypt a message |
| `/api/admin/stats` | GET | Get admin statistics |
| `/api/messages` | GET | Get all encrypted messages |
| `/api/admin/simulate-attack` | POST | Simulate brute force attack |

## Features

- **Message Encryption**: AES-256-CBC with PBKDF2 key derivation
- **Honey Encryption**: Wrong passwords return believable fake plaintexts
- **Fake Plaintext Generation**: Three formats (Chat, Credentials, Email)
- **Admin Dashboard**: Real-time analytics and attack simulation
- **Database Storage**: PostgreSQL for encrypted messages and logs

## Security Principle

Honey Encryption ensures that:
1. Correct password → Real plaintext
2. Incorrect password → Fake plaintext (indistinguishable from real)
3. No correctness signal is leaked to attackers

## Project Structure

```
honey-encryption-system/
├── backend/
│   ├── main.py              # FastAPI application
│   ├── api/routes.py        # API endpoints
│   ├── crypto/
│   │   ├── honey_crypto.py  # Encryption module
│   │   └── fake_generator.py # Fake plaintext generation
│   └── database/
│       ├── models.py        # SQLAlchemy models
│       └── db.py            # Database connection
├── frontend/
│   ├── src/
│   │   ├── pages/          # React pages
│   │   ├── services/       # API services
│   │   └── App.jsx         # Main app component
│   └── package.json
└── database/
    └── schema.sql           # PostgreSQL schema
```

## Demonstration

1. **Encrypt a message**: Go to the Encrypt page, enter a plaintext and password
2. **Decrypt with correct password**: Get the original message
3. **Decrypt with wrong password**: Get a fake plaintext - the attacker cannot tell it's fake!
4. **Simulate attack**: Use the admin dashboard to simulate a brute force attack

## License

Academic project for Cryptography and Network Security course.

