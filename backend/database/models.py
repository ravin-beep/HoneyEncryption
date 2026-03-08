"""
Database models for Honey Encryption System
"""

import uuid
from datetime import datetime
from sqlalchemy import Column, String, Boolean, DateTime, Text, Integer, ForeignKey
from sqlalchemy.orm import relationship
from .db import Base


class Message(Base):
    """Encrypted message storage"""
    __tablename__ = "messages"
    
    id = Column(String(36), primary_key=True, default=lambda: str(uuid.uuid4()))
    ciphertext = Column(Text, nullable=False)
    seed_hash = Column(String(64), nullable=False)  # SHA-256 hex of seed
    created_at = Column(DateTime, default=datetime.utcnow)
    
    # Relationship with decrypt logs
    decrypt_logs = relationship("DecryptLog", back_populates="message", cascade="all, delete-orphan")
    
    def __repr__(self):
        return f"<Message(id={self.id}, created_at={self.created_at})>"


class DecryptLog(Base):
    """Log of decryption attempts"""
    __tablename__ = "decrypt_logs"
    
    id = Column(String(36), primary_key=True, default=lambda: str(uuid.uuid4()))
    message_id = Column(String(36), ForeignKey("messages.id"), nullable=True)
    attempt_time = Column(DateTime, default=datetime.utcnow)
    was_real = Column(Boolean, default=False)
    
    # Relationship with message
    message = relationship("Message", back_populates="decrypt_logs")
    
    def __repr__(self):
        return f"<DecryptLog(id={self.id}, was_real={self.was_real})>"

