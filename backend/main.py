"""
Honey Encryption System - FastAPI Backend

This is the main application file for the Honey Encryption backend.
It provides REST API endpoints for encryption, decryption, and admin functions.
"""

import sys
import os
from pathlib import Path

# Add the backend directory to the path
backend_dir = Path(__file__).parent
sys.path.insert(0, str(backend_dir))

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from contextlib import asynccontextmanager

from api.routes import router
from database.db import init_db


@asynccontextmanager
async def lifespan(app: FastAPI):
    """Application lifespan handler"""
    # Startup: Initialize database
    try:
        init_db()
        print("Database initialized successfully")
    except Exception as e:
        print(f"Database initialization warning: {e}")
    
    yield
    
    # Shutdown: cleanup if needed
    print("Shutting down Honey Encryption API")


# Create FastAPI application
app = FastAPI(
    title="Honey Encryption API",
    description="A deception-based cryptography system that returns plausible fake plaintexts for incorrect passwords",
    version="1.0.0",
    lifespan=lifespan
)

# Configure CORS
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # In production, specify exact origins
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Include routers
app.include_router(router, prefix="/api", tags=["Honey Encryption"])


@app.get("/")
def root():
    """Root endpoint"""
    return {
        "message": "Honey Encryption API",
        "description": "Deception-based cryptography system",
        "version": "1.0.0",
        "docs": "/docs"
    }


@app.get("/health")
def health_check():
    """Health check endpoint"""
    return {"status": "healthy"}


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8001)

