import axios from 'axios'

const API_BASE_URL = import.meta.env.VITE_API_URL || 'http://localhost:8001/api'

const api = axios.create({
  baseURL: API_BASE_URL,
  headers: {
    'Content-Type': 'application/json'
  }
})

// Encrypt a message
export const encryptMessage = async (plaintext, password) => {
  const response = await api.post('/encrypt', { plaintext, password })
  return response.data
}

// Decrypt a message
export const decryptMessage = async (ciphertext, password) => {
  const response = await api.post('/decrypt', { ciphertext, password })
  return response.data
}

// Get admin stats
export const getAdminStats = async () => {
  const response = await api.get('/admin/stats')
  return response.data
}

// Get all messages
export const getMessages = async () => {
  const response = await api.get('/messages')
  return response.data
}

// Simulate attack
export const simulateAttack = async (messageId = null) => {
  const response = await api.post('/admin/simulate-attack', null, {
    params: messageId ? { message_id: messageId } : {}
  })
  return response.data
}

export default api

