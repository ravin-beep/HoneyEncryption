-- Honey Encryption Database Schema
-- PostgreSQL

-- Enable UUID extension
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- Messages table - stores encrypted messages
CREATE TABLE IF NOT EXISTS messages (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    ciphertext TEXT NOT NULL,
    seed_hash VARCHAR(64) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Decrypt logs table - tracks decryption attempts
CREATE TABLE IF NOT EXISTS decrypt_logs (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    message_id UUID REFERENCES messages(id) ON DELETE SET NULL,
    attempt_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    was_real BOOLEAN DEFAULT FALSE
);

-- Create indexes for performance
CREATE INDEX IF NOT EXISTS idx_messages_seed_hash ON messages(seed_hash);
CREATE INDEX IF NOT EXISTS idx_messages_created_at ON messages(created_at DESC);
CREATE INDEX IF NOT EXISTS idx_decrypt_logs_message_id ON decrypt_logs(message_id);
CREATE INDEX IF NOT EXISTS idx_decrypt_logs_was_real ON decrypt_logs(was_real);
CREATE INDEX IF NOT EXISTS idx_decrypt_logs_attempt_time ON decrypt_logs(attempt_time);

-- Admin stats table (optional - can be computed)
CREATE TABLE IF NOT EXISTS admin_stats (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    total_decrypt_attempts VARCHAR(20) DEFAULT '0',
    fake_responses VARCHAR(20) DEFAULT '0',
    real_responses VARCHAR(20) DEFAULT '0',
    simulated_attacks VARCHAR(20) DEFAULT '0',
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- View for getting current stats
CREATE OR REPLACE VIEW decrypt_stats AS
SELECT 
    COUNT(*) as total_attempts,
    COUNT(*) FILTER (WHERE was_real = FALSE) as fake_responses,
    COUNT(*) FILTER (WHERE was_real = TRUE) as real_responses
FROM decrypt_logs;

