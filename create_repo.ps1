$env:Path += ";$env:ProgramFiles\GitHub CLI"
cd "c:/Users/Rahul/OneDrive/Desktop/Year3/CNS\HoneyEncryption"

# Initialize git if not already initialized
if (!(Test-Path ".git")) {
    git init
    git config user.email "rahul@example.com"
    git config user.name "Rahul"
    
    # Add all files except build artifacts
    git add -A
    
    # Exclude build files and temporary files
    git reset HEAD build/ honey_encrypt honey_encrypt.exe mappings.txt create_repo.ps1 build_*.ps1 test_*.ps1
    
    # Commit
    git commit -m "Initial commit - Honey Encryption System"
}

# Create the repository and push
gh repo create HoneyEncryption --public --source=. --description "Honey Encryption - A security system that on incorrect password attempts" --push
 returns fake plaintexts
