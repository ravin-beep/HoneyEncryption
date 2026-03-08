"""
Fake Plaintext Generator

Generates realistic-looking fake plaintexts for honey encryption:
- Chat messages
- Credentials (username/password/email)
- Email format messages
"""

import random
import string


class FakeGenerator:
    """Generates fake plaintexts for honey encryption"""
    
    def __init__(self):
        self._init_data()
    
    def _init_data(self):
        """Initialize data lists for fake generation"""
        
        # Chat message templates
        self.chat_templates = [
            "Hey, let's meet {time} {day}.",
            "Did you see the {topic}?",
            "Can you send me the {item}?",
            "The meeting is at {time}.",
            "I'll send you the {item} later.",
            "Thanks for the {item}!",
            "Sure, no problem!",
            "Let me check and get back to you.",
            "The {topic} looks great!",
            "I'll be there at {time}.",
            "Don't forget the {item}!",
            "See you {day}!",
            "Great work on the {topic}!",
            "Can you call me later?",
            "Just finished the {item}.",
            "The {item} is ready.",
            "Let's discuss this {day}.",
            "Perfect, that works for me!",
            "I'll {action} tomorrow.",
            "Thanks for your help!"
        ]
        
        # Chat placeholders
        self.times = ["9am", "10am", "2pm", "3pm", "4pm", "5pm", "morning", "afternoon", "evening"]
        self.days = ["today", "tomorrow", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"]
        self.topics = ["project", "report", "presentation", "meeting", "document", "email", "message", "update"]
        self.items = ["file", "document", "report", "email", "message", "details", "info", "data", "results"]
        self.actions = ["send it", "finish it", "complete it", "review it", "check it", "update it"]
        
        # First names
        self.first_names = [
            "James", "John", "Robert", "Michael", "William", "David", "Richard", 
            "Joseph", "Thomas", "Charles", "Mary", "Patricia", "Jennifer", "Linda", 
            "Barbara", "Elizabeth", "Susan", "Jessica", "Sarah", "Karen", "Lisa",
            "Nancy", "Betty", "Margaret", "Sandra", "Ashley", "Kimberly", "Emily",
            "Donna", "Michelle", "Carol", "Amanda", "Melissa", "Deborah", "Stephanie"
        ]
        
        # Last names
        self.last_names = [
            "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", 
            "Davis", "Rodriguez", "Martinez", "Hernandez", "Lopez", "Gonzalez", 
            "Wilson", "Anderson", "Thomas", "Taylor", "Moore", "Jackson", "Martin",
            "Lee", "Perez", "Thompson", "White", "Harris", "Sanchez", "Clark"
        ]
        
        # Email domains
        self.email_domains = [
            "gmail.com", "yahoo.com", "outlook.com", "hotmail.com", 
            "mail.com", "example.com", "company.com", "business.org"
        ]
        
        # Email subjects
        self.email_subjects = [
            "Meeting Update", "Project Status", "Quick Question", "Follow Up",
            "Re: Document", "Important", "Information", "Request",
            "Confirmation", "Reminder", "Thanks", "Update on Matter"
        ]
        
        # Email message starters
        self.email_starters = [
            "I wanted to follow up on", "Please find the", "Thank you for your",
            "I hope this email", "Just a quick", "As discussed", 
            "Following up on our", "I am writing to"
        ]
        
        # Single words dictionary
        self.word_dictionary = [
            "account", "admin", "balance", "bank", "billing", "budget", "card",
            "cash", "client", "cloud", "code", "config", "credit", "crypto",
            "customer", "data", "database", "domain", "download", "driver",
            "email", "encryption", "file", "folder", "gateway", "hardware",
            "host", "image", "invoice", "key", "license", "login", "mail",
            "network", "note", "order", "packet", "password", "payment", "phone",
            "portal", "private", "profile", "project", "proxy", "public",
            "query", "record", "report", "request", "resource", "router",
            "script", "secure", "server", "service", "session", "share",
            "software", "source", "storage", "stream", "system", "token",
            "traffic", "transaction", "upload", "user", "video", "voice",
            "wallet", "web", "wireless", "worker", "zone", "ready", "done",
            "sent", "received", "approved", "pending", "waiting", "processing"
        ]
    
    def _generate_random_password(self, length: int = 12) -> str:
        """Generate a random password-like string"""
        chars = string.ascii_lowercase + string.ascii_uppercase + string.digits + "!@#$%"
        return ''.join(random.choice(chars) for _ in range(length))
    
    def _generate_random_username(self) -> str:
        """Generate a random username"""
        first = random.choice(self.first_names).lower()
        last = random.choice(self.last_names).lower()
        num = random.randint(1, 999)
        
        patterns = [
            f"{first}{last}",
            f"{first}.{last}",
            f"{first}_{last}",
            f"{first}{num}",
            f"{last}{first}",
            f"{first[0]}{last}{num}"
        ]
        return random.choice(patterns)
    
    def _generate_random_email(self) -> str:
        """Generate a random email address"""
        username = self._generate_random_username()
        domain = random.choice(self.email_domains)
        return f"{username}@{domain}"
    
    def generate_chat_message(self) -> str:
        """Generate a fake chat message"""
        template = random.choice(self.chat_templates)
        
        replacements = {
            "{time}": random.choice(self.times),
            "{day}": random.choice(self.days),
            "{topic}": random.choice(self.topics),
            "{item}": random.choice(self.items),
            "{action}": random.choice(self.actions)
        }
        
        for key, value in replacements.items():
            template = template.replace(key, value)
        
        return template
    
    def generate_credentials(self) -> str:
        """Generate fake credentials"""
        username = self._generate_random_username()
        password = self._generate_random_password()
        email = self._generate_random_email()
        
        return f"USERNAME: {username}\nPASSWORD: {password}\nEMAIL: {email}"
    
    def generate_email(self) -> str:
        """Generate fake email format"""
        subject = random.choice(self.email_subjects)
        starter = random.choice(self.email_starters)
        topic = random.choice(self.topics)
        
        message = f"{starter} the {topic}. Let me know if you need any additional information."
        
        return f"Subject: {subject}\nMessage: {message}"
    
    def generate_fake_plaintext(self) -> str:
        """Generate a fake plaintext using one of three strategies"""
        choice = random.randint(0, 2)
        
        if choice == 0:
            return self.generate_chat_message()
        elif choice == 1:
            return self.generate_credentials()
        else:
            return self.generate_email()
    
    def generate_fake_word(self) -> str:
        """Generate a single realistic word"""
        return random.choice(self.word_dictionary)


# Singleton instance
_fake_generator = None

def get_fake_generator() -> FakeGenerator:
    """Get the singleton FakeGenerator instance"""
    global _fake_generator
    if _fake_generator is None:
        _fake_generator = FakeGenerator()
    return _fake_generator


def generate_fake_plaintext() -> str:
    """Generate a fake plaintext"""
    return get_fake_generator().generate_fake_plaintext()


def generate_fake_word() -> str:
    """Generate a single fake word"""
    return get_fake_generator().generate_fake_word()
