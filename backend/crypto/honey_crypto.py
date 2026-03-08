"""
Honey Encryption Module

This module implements the core honey encryption logic:
- Encrypt messages using password-derived AES-256 keys
- Decrypt messages and validate against stored seeds
- Return fake plaintext for incorrect passwords
"""

import hashlib
import os
import base64
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC


class HoneyCrypto:
    """Honey Encryption handler"""
    
    def __init__(self):
        self.backend = default_backend()
        self.salt = b"honey_encryption_salt"  # In production, use unique salt per message
    
    def derive_key(self, password: str) -> bytes:
        """
        Derive AES-256 key from password using PBKDF2
        
        Args:
            password: User password
            
        Returns:
            32-byte AES key
        """
        kdf = PBKDF2HMAC(
            algorithm=hashes.SHA256(),
            length=32,
            salt=self.salt,
            iterations=100000,
            backend=self.backend
        )
        return kdf.derive(password.encode())
    
    def generate_seed(self, plaintext: str) -> bytes:
        """
        Generate deterministic seed from plaintext using SHA-256
        
        Args:
            plaintext: The plaintext message
            
        Returns:
            32-byte deterministic seed
        """
        return hashlib.sha256(plaintext.encode()).digest()
    
    def encrypt_seed(self, seed: bytes, password: str) -> str:
        """
        Encrypt seed using AES-256-CBC
        
        Args:
            seed: 32-byte seed to encrypt
            password: User password
            
        Returns:
            Base64-encoded ciphertext (IV + ciphertext)
        """
        key = self.derive_key(password)
        iv = os.urandom(16)  # Random IV
        
        cipher = Cipher(
            algorithms.AES(key),
            modes.CBC(iv),
            backend=self.backend
        )
        encryptor = cipher.encryptor()
        
        # Pad seed to block size
        padded_seed = self._pad(seed)
        ciphertext = encryptor.update(padded_seed) + encryptor.finalize()
        
        # Return IV + ciphertext as base64
        return base64.b64encode(iv + ciphertext).decode()
    
    def decrypt_seed(self, ciphertext: str, password: str) -> bytes:
        """
        Decrypt seed using AES-256-CBC
        
        Args:
            ciphertext: Base64-encoded ciphertext
            password: User password
            
        Returns:
            Decrypted seed
        """
        key = self.derive_key(password)
        
        # Decode base64
        data = base64.b64decode(ciphertext)
        iv = data[:16]
        encrypted_data = data[16:]
        
        cipher = Cipher(
            algorithms.AES(key),
            modes.CBC(iv),
            backend=self.backend
        )
        decryptor = cipher.decryptor()
        
        padded_seed = decryptor.update(encrypted_data) + decryptor.finalize()
        return self._unpad(padded_seed)
    
    def _pad(self, data: bytes) -> bytes:
        """PKCS7 padding"""
        padding_length = 16 - (len(data) % 16)
        padding = bytes([padding_length] * padding_length)
        return data + padding
    
    def _unpad(self, data: bytes) -> bytes:
        """Remove PKCS7 padding"""
        padding_length = data[-1]
        return data[:-padding_length]
    
    def encrypt_message(self, plaintext: str, password: str) -> str:
        """
        Encrypt a plaintext message
        
        Args:
            plaintext: Message to encrypt
            password: Password for encryption
            
        Returns:
            Hex-encoded ciphertext
        """
        # Generate deterministic seed from plaintext
        seed = self.generate_seed(plaintext)
        
        # Encrypt the seed using AES-256-CBC
        ciphertext = self.encrypt_seed(seed, password)
        
        return ciphertext
    
    def decrypt_message(self, ciphertext: str, password: str) -> tuple[str, bool]:
        """
        Decrypt a ciphertext message
        
        Returns plaintext (real or fake) and whether it's the real one.
        
        Args:
            ciphertext: Encrypted message
            password: Password for decryption
            
        Returns:
            Tuple of (plaintext, is_real)
        """
        try:
            # Decrypt to get the seed
            seed = self.decrypt_seed(ciphertext, password)
            seed_hex = seed.hex()
            
            # Return the seed for lookup - actual validation happens in the API layer
            # This allows us to always return some plaintext
            return seed_hex, True
            
        except Exception as e:
            # Decryption failed - return fake
            from .fake_generator import FakeGenerator
            fg = FakeGenerator()
            return fg.generate_fake_plaintext(), False
    
    def get_seed_from_ciphertext(self, ciphertext: str, password: str) -> bytes | None:
        """
        Try to decrypt and get the seed from ciphertext
        
        Args:
            ciphertext: Encrypted message
            password: Password to try
            
        Returns:
            Decrypted seed bytes or None if decryption fails
        """
        try:
            return self.decrypt_seed(ciphertext, password)
        except Exception:
            return None


# Singleton instance
_honey_crypto = None

def get_honey_crypto() -> HoneyCrypto:
    """Get the singleton HoneyCrypto instance"""
    global _honey_crypto
    if _honey_crypto is None:
        _honey_crypto = HoneyCrypto()
    return _honey_crypto

