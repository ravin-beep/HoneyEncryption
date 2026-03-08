"""
Database connection and session management
"""

import os
from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from dotenv import load_dotenv

# Load environment variables
load_dotenv()

# Check if PostgreSQL is available, fallback to SQLite
USE_SQLITE = os.getenv("USE_SQLITE", "true").lower() == "true"

if USE_SQLITE:
    # Use SQLite for local development (no PostgreSQL setup required)
    DATABASE_URL = "sqlite:///./honey_encryption.db"
    engine = create_engine(
        DATABASE_URL, 
        echo=False,
        connect_args={"check_same_thread": False}
    )
else:
    # Use PostgreSQL
    DATABASE_URL = os.getenv(
        "DATABASE_URL",
        "postgresql+psycopg2://postgres:rahul123@localhost:5432/honey_encryption"
    )
    engine = create_engine(DATABASE_URL, echo=False)

# Create session factory
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

# Base class for models
Base = declarative_base()


def get_db():
    """Get database session"""
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


def init_db():
    """Initialize database tables"""
    from .models import Message, DecryptLog
    Base.metadata.create_all(bind=engine)

