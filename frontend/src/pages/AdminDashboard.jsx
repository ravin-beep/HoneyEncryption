import { useState, useEffect } from 'react'
import { getAdminStats, getMessages, simulateAttack } from '../services/api'
import { PieChart, Pie, Cell, BarChart, Bar, XAxis, YAxis, Tooltip, Legend, ResponsiveContainer } from 'recharts'

function AdminDashboard() {
  const [stats, setStats] = useState({
    total_decrypt_attempts: 0,
    fake_responses: 0,
    real_responses: 0,
    simulated_attacks: 0
  })
  const [messages, setMessages] = useState([])
  const [loading, setLoading] = useState(true)
  const [simulating, setSimulating] = useState(false)
  const [attackResult, setAttackResult] = useState(null)

  useEffect(() => {
    fetchData()
  }, [])

  const fetchData = async () => {
    try {
      const [statsData, messagesData] = await Promise.all([
        getAdminStats(),
        getMessages()
      ])
      setStats(statsData)
      setMessages(messagesData)
    } catch (err) {
      console.error('Failed to fetch data:', err)
    } finally {
      setLoading(false)
    }
  }

  const handleSimulateAttack = async () => {
    setSimulating(true)
    setAttackResult(null)
    
    try {
      const result = await simulateAttack()
      setAttackResult(result)
      await fetchData() // Refresh stats
    } catch (err) {
      console.error('Attack simulation failed:', err)
    } finally {
      setSimulating(false)
    }
  }

  const pieData = [
    { name: 'Fake Responses', value: stats.fake_responses, color: '#f59e0b' },
    { name: 'Real Responses', value: stats.real_responses, color: '#22c55e' }
  ]

  const barData = [
    { name: 'Total Attempts', value: stats.total_decrypt_attempts },
    { name: 'Fake Responses', value: stats.fake_responses },
    { name: 'Real Responses', value: stats.real_responses },
    { name: 'Simulated Attacks', value: stats.simulated_attacks }
  ]

  if (loading) {
    return (
      <div className="flex items-center justify-center h-64">
        <div className="text-honey-400 text-xl">Loading dashboard...</div>
      </div>
    )
  }

  return (
    <div className="space-y-6">
      <div className="flex items-center justify-between">
        <h1 className="text-3xl font-bold text-honey-400">Admin Dashboard</h1>
        <button
          onClick={handleSimulateAttack}
          disabled={simulating || messages.length === 0}
          className="btn-primary disabled:opacity-50"
        >
          {simulating ? 'Simulating...' : 'Simulate Attack'}
        </button>
      </div>

      {attackResult && (
        <div className="bg-honey-900 border border-honey-700 text-honey-200 px-4 py-3 rounded">
          Attack Simulation Complete! Tried {attackResult.attempts} random passwords - 
          all returned fake plaintexts.
        </div>
      )}

      {messages.length === 0 && (
        <div className="bg-yellow-900 border border-yellow-700 text-yellow-200 px-4 py-3 rounded">
          No messages in database. Encrypt some messages first to see analytics.
        </div>
      )}

      {/* Stats Cards */}
      <div className="grid grid-cols-2 md:grid-cols-4 gap-4">
        <div className="card">
          <div className="text-2xl font-bold text-white">{stats.total_decrypt_attempts}</div>
          <div className="text-gray-400 text-sm">Total Decrypt Attempts</div>
        </div>
        <div className="card">
          <div className="text-2xl font-bold text-honey-400">{stats.fake_responses}</div>
          <div className="text-gray-400 text-sm">Fake Responses</div>
        </div>
        <div className="card">
          <div className="text-2xl font-bold text-green-400">{stats.real_responses}</div>
          <div className="text-gray-400 text-sm">Real Responses</div>
        </div>
        <div className="card">
          <div className="text-2xl font-bold text-blue-400">{stats.simulated_attacks}</div>
          <div className="text-gray-400 text-sm">Simulated Attacks</div>
        </div>
      </div>

      {/* Charts */}
      <div className="grid md:grid-cols-2 gap-6">
        <div className="card">
          <h2 className="text-xl font-bold text-honey-400 mb-4">Response Distribution</h2>
          <ResponsiveContainer width="100%" height={250}>
            <PieChart>
              <Pie
                data={pieData}
                cx="50%"
                cy="50%"
                innerRadius={60}
                outerRadius={80}
                paddingAngle={5}
                dataKey="value"
                label
              >
                {pieData.map((entry, index) => (
                  <Cell key={`cell-${index}`} fill={entry.color} />
                ))}
              </Pie>
              <Tooltip />
              <Legend />
            </PieChart>
          </ResponsiveContainer>
        </div>

        <div className="card">
          <h2 className="text-xl font-bold text-honey-400 mb-4">Statistics Overview</h2>
          <ResponsiveContainer width="100%" height={250}>
            <BarChart data={barData}>
              <XAxis dataKey="name" tick={{ fill: '#9ca3af' }} />
              <YAxis tick={{ fill: '#9ca3af' }} />
              <Tooltip />
              <Bar dataKey="value" fill="#f59e0b" />
            </BarChart>
          </ResponsiveContainer>
        </div>
      </div>

      {/* Messages Table */}
      <div className="card">
        <h2 className="text-xl font-bold text-honey-400 mb-4">Encrypted Messages</h2>
        {messages.length === 0 ? (
          <p className="text-gray-400">No messages stored yet.</p>
        ) : (
          <div className="overflow-x-auto">
            <table className="w-full text-sm">
              <thead>
                <tr className="border-b border-gray-700">
                  <th className="text-left py-3 px-2 text-gray-400">ID</th>
                  <th className="text-left py-3 px-2 text-gray-400">Ciphertext</th>
                  <th className="text-left py-3 px-2 text-gray-400">Seed Hash</th>
                  <th className="text-left py-3 px-2 text-gray-400">Created</th>
                </tr>
              </thead>
              <tbody>
                {messages.map((msg) => (
                  <tr key={msg.id} className="border-b border-gray-700">
                    <td className="py-3 px-2 font-mono text-xs">{msg.id.substring(0, 8)}...</td>
                    <td className="py-3 px-2 font-mono text-xs truncate max-w-xs">{msg.ciphertext}</td>
                    <td className="py-3 px-2 font-mono text-xs">{msg.seed_hash.substring(0, 16)}...</td>
                    <td className="py-3 px-2 text-gray-400">{new Date(msg.created_at).toLocaleString()}</td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        )}
      </div>

      {/* Security Note */}
      <div className="card bg-gray-800">
        <h3 className="font-bold text-honey-400 mb-2">Security Note</h3>
        <p className="text-gray-400 text-sm">
          This dashboard shows that attackers cannot determine if their decryption attempts are successful.
          Every wrong password returns a fake plaintext, creating uncertainty and alerting the system to
          potential attacks through multiple decryption attempts.
        </p>
      </div>
    </div>
  )
}

export default AdminDashboard

