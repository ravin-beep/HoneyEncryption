import { Link } from 'react-router-dom'

function Home() {
  return (
    <div className="space-y-8">
      {/* Hero Section */}
      <div className="text-center py-12">
        <div className="text-6xl mb-4">🍯</div>
        <h1 className="text-4xl font-bold text-honey-400 mb-4">
          Honey Encryption
        </h1>
        <p className="text-xl text-gray-300 max-w-2xl mx-auto">
          Security through deception. Every decryption attempt returns plausible output,
          making it impossible for attackers to know if they've succeeded.
        </p>
      </div>

      {/* Key Features */}
      <div className="grid md:grid-cols-3 gap-6">
        <div className="card">
          <div className="text-3xl mb-3">🔐</div>
          <h3 className="text-xl font-bold text-honey-400 mb-2">AES-256 Encryption</h3>
          <p className="text-gray-400">
            Military-grade encryption with secure key derivation using PBKDF2.
          </p>
        </div>
        
        <div className="card">
          <div className="text-3xl mb-3">🎭</div>
          <h3 className="text-xl font-bold text-honey-400 mb-2">Deception Layer</h3>
          <p className="text-gray-400">
            Incorrect passwords return realistic fake plaintexts (honeywords).
          </p>
        </div>
        
        <div className="card">
          <div className="text-3xl mb-3">📊</div>
          <h3 className="text-xl font-bold text-honey-400 mb-2">Attack Analytics</h3>
          <p className="text-gray-400">
            Monitor decryption attempts and detect brute force attacks.
          </p>
        </div>
      </div>

      {/* How It Works */}
      <div className="card">
        <h2 className="text-2xl font-bold text-honey-400 mb-4">How It Works</h2>
        <div className="space-y-4 text-gray-300">
          <div className="flex items-start space-x-4">
            <div className="flex-shrink-0 w-8 h-8 bg-honey-500 rounded-full flex items-center justify-center font-bold">1</div>
            <div>
              <h4 className="font-bold">Encrypt a Message</h4>
              <p className="text-gray-400">Enter your plaintext and a password. The system generates a unique seed and encrypts it.</p>
            </div>
          </div>
          
          <div className="flex items-start space-x-4">
            <div className="flex-shrink-0 w-8 h-8 bg-honey-500 rounded-full flex items-center justify-center font-bold">2</div>
            <div>
              <h4 className="font-bold">Decrypt with Correct Password</h4>
              <p className="text-gray-400">Using the correct password returns the original plaintext.</p>
            </div>
          </div>
          
          <div className="flex items-start space-x-4">
            <div className="flex-shrink-0 w-8 h-8 bg-honey-500 rounded-full flex items-center justify-center font-bold">3</div>
            <div>
              <h4 className="font-bold">Decrypt with Wrong Password</h4>
              <p className="text-gray-400">Using any incorrect password returns a believable fake plaintext - the attacker has no way to know they failed!</p>
            </div>
          </div>
        </div>
      </div>

      {/* Quick Actions */}
      <div className="flex justify-center space-x-4">
        <Link to="/encrypt" className="btn-primary">
          Encrypt a Message
        </Link>
        <Link to="/decrypt" className="btn-secondary">
          Decrypt a Message
        </Link>
      </div>
    </div>
  )
}

export default Home

