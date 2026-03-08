import { useState } from 'react'
import { decryptMessage } from '../services/api'

function Decrypt() {
  const [ciphertext, setCiphertext] = useState('')
  const [password, setPassword] = useState('')
  const [plaintext, setPlaintext] = useState('')
  const [isReal, setIsReal] = useState(null)
  const [error, setError] = useState('')
  const [loading, setLoading] = useState(false)

  const handleDecrypt = async (e) => {
    e.preventDefault()
    setError('')
    setPlaintext('')
    setIsReal(null)

    if (!ciphertext.trim()) {
      setError('Please enter a ciphertext')
      return
    }

    if (!password) {
      setError('Please enter a password')
      return
    }

    setLoading(true)

    try {
      const result = await decryptMessage(ciphertext, password)
      setPlaintext(result.plaintext)
      setIsReal(result.is_real)
    } catch (err) {
      // Even on error, we get a fake plaintext (honey encryption principle)
      setPlaintext(err.response?.data?.plaintext || 'Decryption failed')
      setIsReal(false)
    } finally {
      setLoading(false)
    }
  }

  return (
    <div className="max-w-2xl mx-auto">
      <h1 className="text-3xl font-bold text-honey-400 mb-6">Decrypt Message</h1>
      
      <div className="card">
        <form onSubmit={handleDecrypt} className="space-y-4">
          <div>
            <label className="block text-sm font-medium mb-2">
              Ciphertext
            </label>
            <textarea
              value={ciphertext}
              onChange={(e) => setCiphertext(e.target.value)}
              className="input-field h-32 resize-none font-mono text-xs"
              placeholder="Enter the ciphertext to decrypt..."
            />
          </div>

          <div>
            <label className="block text-sm font-medium mb-2">
              Password
            </label>
            <input
              type="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              className="input-field"
              placeholder="Enter the password"
            />
          </div>

          {error && (
            <div className="bg-red-900 border border-red-700 text-red-200 px-4 py-3 rounded">
              {error}
            </div>
          )}

          <button
            type="submit"
            disabled={loading}
            className="w-full btn-primary disabled:opacity-50"
          >
            {loading ? 'Decrypting...' : 'Decrypt Message'}
          </button>
        </form>

        {plaintext && (
          <div className={`mt-6 p-4 rounded-lg ${isReal ? 'bg-green-900' : 'bg-gray-700'}`}>
            <div className="flex items-center justify-between mb-3">
              <h3 className="text-lg font-bold text-white">Decryption Result</h3>
              {isReal ? (
                <span className="px-3 py-1 bg-green-600 text-white text-sm rounded-full">
                  Real Plaintext ✓
                </span>
              ) : (
                <span className="px-3 py-1 bg-honey-600 text-white text-sm rounded-full">
                  Fake Plaintext (Honey)
                </span>
              )}
            </div>
            
            <div className="bg-gray-800 p-4 rounded">
              <pre className="whitespace-pre-wrap text-gray-200 font-mono text-sm">
                {plaintext}
              </pre>
            </div>

            <div className="mt-3 text-sm text-gray-400">
              {isReal ? (
                <p>✓ The correct password was used. This is the original plaintext.</p>
              ) : (
                <p>⚠ The password was incorrect. The system returned a believable fake plaintext. 
                An attacker cannot distinguish this from a real decryption!</p>
              )}
            </div>
          </div>
        )}
      </div>

      <div className="mt-6 p-4 bg-gray-800 rounded-lg">
        <h3 className="font-bold text-honey-400 mb-2">Honey Encryption Demo</h3>
        <p className="text-gray-400 text-sm mb-3">
          Try entering different wrong passwords. Each time, you'll get a different believable fake message.
          This demonstrates the power of honey encryption - there's no way to know if decryption succeeded!
        </p>
        <ul className="text-gray-400 text-sm list-disc list-inside space-y-1">
          <li>Same ciphertext + wrong password = believable fake plaintext</li>
          <li>Same ciphertext + correct password = real plaintext</li>
          <li>Attacker can't tell the difference!</li>
        </ul>
      </div>
    </div>
  )
}

export default Decrypt

