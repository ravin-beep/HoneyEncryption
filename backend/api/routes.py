"""
API routes for Honey Encryption System
"""

import sys
import os
from pathlib import Path

# Add the backend directory to the path
backend_dir = Path(__file__).parent.parent
sys.path.insert(0, str(backend_dir))

from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session
from typing import Optional
from datetime import datetime
import random
import string

from database import get_db, Message, DecryptLog
from crypto.honey_crypto import get_honey_crypto
from crypto.fake_generator import generate_fake_plaintext

# Create router
router = APIRouter()

# Get crypto instance
honey_crypto = get_honey_crypto()


# Request body models (using dict instead of Pydantic)
@router.post("/encrypt")
def encrypt(request: dict, db: Session = Depends(get_db)):
    """
    Encrypt a plaintext message
    
    Returns ciphertext and stores metadata in database
    """
    plaintext = request.get("plaintext", "")
    password = request.get("password", "")
    
    if not plaintext:
        raise HTTPException(status_code=400, detail="Plaintext cannot be empty")
    if not password:
        raise HTTPException(status_code=400, detail="Password cannot be empty")
    
    try:
        # Generate seed and encrypt
        seed = honey_crypto.generate_seed(plaintext)
        seed_hash = seed.hex()
        ciphertext = honey_crypto.encrypt_seed(seed, password)
        
        # Store in database
        db_message = Message(
            ciphertext=ciphertext,
            seed_hash=seed_hash
        )
        db.add(db_message)
        db.commit()
        db.refresh(db_message)
        
        return {
            "ciphertext": ciphertext,
            "message_id": str(db_message.id),
            "seed_hash": seed_hash
        }
    
    except Exception as e:
        db.rollback()
        raise HTTPException(status_code=500, detail=f"Encryption failed: {str(e)}")


@router.post("/decrypt")
def decrypt(request: dict, db: Session = Depends(get_db)):
    """
    Decrypt a ciphertext message
    
    Returns real plaintext if password is correct,
    fake plaintext if password is incorrect
    """
    ciphertext = request.get("ciphertext", "")
    password = request.get("password", "")
    
    if not ciphertext:
        raise HTTPException(status_code=400, detail="Ciphertext cannot be empty")
    if not password:
        raise HTTPException(status_code=400, detail="Password cannot be empty")
    
    try:
        # Try to decrypt
        seed = honey_crypto.decrypt_seed(ciphertext, password)
        seed_hex = seed.hex()
        
        # Check if seed matches any stored message
        message = db.query(Message).filter(Message.seed_hash == seed_hex).first()
        
        if message:
            # Password is correct - return real plaintext
            plaintext = f"Decrypted message (seed: {seed_hex[:16]}...)"
            is_real = True
            
            # Log the attempt
            log = DecryptLog(
                message_id=message.id,
                was_real=True
            )
            db.add(log)
            db.commit()
            
            return {"plaintext": plaintext, "is_real": is_real}
        else:
            # Password is incorrect - return fake plaintext
            plaintext = generate_fake_plaintext()
            is_real = False
            
            # Log the attempt
            log = DecryptLog(was_real=False)
            db.add(log)
            db.commit()
            
            return {"plaintext": plaintext, "is_real": is_real}
    
    except Exception as e:
        # Decryption failed - return fake plaintext (honey encryption principle)
        plaintext = generate_fake_plaintext()
        
        # Log the failed attempt
        log = DecryptLog(was_real=False)
        db.add(log)
        db.commit()
        
        return {"plaintext": plaintext, "is_real": False}


@router.get("/admin/stats")
def get_stats(db: Session = Depends(get_db)):
    """Get admin statistics"""
    
    # Get all decrypt logs
    total_attempts = db.query(DecryptLog).count()
    fake_responses = db.query(DecryptLog).filter(DecryptLog.was_real == False).count()
    real_responses = db.query(DecryptLog).filter(DecryptLog.was_real == True).count()
    
    return {
        "total_decrypt_attempts": total_attempts,
        "fake_responses": fake_responses,
        "real_responses": real_responses,
        "simulated_attacks": 0
    }


@router.get("/messages")
def get_messages(db: Session = Depends(get_db)):
    """Get all encrypted messages (admin only)"""
    
    messages = db.query(Message).order_by(Message.created_at.desc()).all()
    
    return [
        {
            "id": str(m.id),
            "ciphertext": m.ciphertext[:100] + "..." if len(m.ciphertext) > 100 else m.ciphertext,
            "seed_hash": m.seed_hash,
            "created_at": m.created_at.isoformat() if m.created_at else None
        }
        for m in messages
    ]


@router.post("/admin/simulate-attack")
def simulate_attack(message_id: Optional[str] = None, db: Session = Depends(get_db)):
    """
    Simulate a brute force attack
    
    Tries random passwords against stored ciphertext
    """
    # Get a message to attack
    message = None
    if message_id:
        message = db.query(Message).filter(Message.id == message_id).first()
    else:
        message = db.query(Message).order_by(Message.created_at.desc()).first()
    
    if not message:
        raise HTTPException(status_code=404, detail="No messages found to attack")
    
    # Generate random passwords and try to decrypt
    fake_responses = 0
    attempts = 100
    
    for _ in range(attempts):
        # Generate random password
        random_password = ''.join(random.choices(string.ascii_letters + string.digits, k=12))
        
        try:
            seed = honey_crypto.decrypt_seed(message.ciphertext, random_password)
            seed_hex = seed.hex()
            
            # Check if it's the correct seed
            if seed_hex == message.seed_hash:
                fake_responses += 0  # This is a real response
            else:
                fake_responses += 1
                
        except Exception:
            fake_responses += 1
        
        # Log each attempt
        log = DecryptLog(
            message_id=message.id,
            was_real=False
        )
        db.add(log)
    
    db.commit()
    
    return {
        "message": "Attack simulation complete",
        "attempts": attempts,
        "fake_responses": fake_responses,
        "real_responses": 0
    }

