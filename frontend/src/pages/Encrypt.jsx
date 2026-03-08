import { useState } from 'react'
import { encryptMessage } from '../services/api'

function Encrypt() {
  const [plaintext, setPlaintext] = useState('')
  const [password, setPassword] = useState('')
  const [confirmPassword, setConfirmPassword] = useState('')
  const [ciphertext, setCiphertext] = useState('')
  const [messageId, setMessageId] = useState('')
  const [seedHash, setSeedHash] = useState('')
  const [error, setError] = useState('')
  const [loading, setLoading] = useState(false)

  const handleEncrypt = async (e) => {
    e.preventDefault()
    setError('')
    setCiphertext('')
    setMessageId('')
    setSeedHash('')

    if (!plaintext.trim()) {
      setError('Please enter a plaintext message')
      return
    }

    if (!password) {
      setError('Please enter a password')
      return
    }

    if (password !== confirmPassword) {
      setError('Passwords do not match')
      return
    }

    if (password.length < 4) {
      setError('Password must be at least 4 characters')
      return
    }

    setLoading(true)

    try {
      const result = await encryptMessage(plaintext, password)
      setCiphertext(result.ciphertext)
      setMessageId(result.message_id)
      setSeedHash(result.seed_hash)
    } catch (err) {
      setError(err.response?.data?.detail || 'Encryption failed. Please try again.')
    } finally {
      setLoading(false)
    }
  }

  const copyToClipboard = () => {
    navigator.clipboard.writeText(ciphertext)
    alert('Ciphertext copied to clipboard!')
  }

  return (
    <div className="max-w-2xl mx-auto">
      <h1 className="text-3xl font-bold text-honey-400 mb-6">Encrypt Message</h1>
      
      <div className="card">
        <form onSubmit={handleEncrypt} className="space-y-4">
          <div>
            <label className="block text-sm font-medium mb-2">
              Plaintext Message
            </label>
            <textarea
              value={plaintext}
              onChange={(e) => setPlaintext(e.target.value)}
              className="input-field h-32 resize-none"
              placeholder="Enter your secret message..."
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
              placeholder="Enter a strong password"
            />
          </div>

          <div>
            <label className="block text-sm font-medium mb-2">
              Confirm Password
            </label>
            <input
              type="password"
              value={confirmPassword}
              onChange={(e) => setConfirmPassword(e.target.value)}
              className="input-field"
              placeholder="Confirm your password"
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
            {loading ? 'Encrypting...' : 'Encrypt Message'}
          </button>
        </form>

        {ciphertext && (
          <div className="mt-6 p-4 bg-gray-700 rounded-lg">
            <h3 className="text-lg font-bold text-honey-400 mb-3">Encryption Successful!</h3>
            
            <div className="space-y-3">
              <div>
                <label className="block text-sm text-gray-400 mb-1">Ciphertext:</label>
                <div className="flex gap-2">
                  <textarea
                    readOnly
                    value={ciphertext}
                    className="input-field flex-1 h-24 text-xs font-mono"
                  />
                  <button
                    onClick={copyToClipboard}
                    className="btn-secondary whitespace-nowrap"
                  >
                    Copy
                  </button>
                </div>
              </div>

              <div className="grid grid-cols-2 gap-4 text-sm">
                <div>
                  <label className="block text-gray-400">Message ID:</label>
                  <code className="text-honey-400">{messageId}</code>
                </div>
                <div>
                  <label className="block text-gray-400">Seed Hash:</label>
                  <code className="text-honey-400 text-xs">{seedHash}</code>
                </div>
              </div>
            </div>
          </div>
        )}
      </div>

      <div className="mt-6 p-4 bg-gray-800 rounded-lg">
        <h3 className="font-bold text-honey-400 mb-2">Security Tips</h3>
        <ul className="text-gray-400 text-sm list-disc list-inside space-y-1">
          <li>Use a strong password with at least 8 characters</li>
          <li>Remember your password - there's no recovery option</li>
          <li>Share the ciphertext, not the password</li>
          <li>The same plaintext with different passwords produces different ciphertext</li>
        </ul>
      </div>
    </div>
  )
}

export default Encrypt

