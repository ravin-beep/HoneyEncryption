import { Routes, Route, Link } from 'react-router-dom'
import Home from './pages/Home'
import Encrypt from './pages/Encrypt'
import Decrypt from './pages/Decrypt'
import AdminDashboard from './pages/AdminDashboard'

function App() {
  return (
    <div className="min-h-screen bg-gray-900 text-white">
      {/* Navigation */}
      <nav className="bg-gray-800 border-b border-gray-700">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
          <div className="flex items-center justify-between h-16">
            <div className="flex items-center">
              <Link to="/" className="flex items-center space-x-2">
                <span className="text-2xl">🍯</span>
                <span className="text-xl font-bold text-honey-400">Honey Encryption</span>
              </Link>
            </div>
            <div className="flex space-x-4">
              <Link to="/" className="px-3 py-2 rounded-md text-sm font-medium hover:bg-gray-700">
                Home
              </Link>
              <Link to="/encrypt" className="px-3 py-2 rounded-md text-sm font-medium hover:bg-gray-700">
                Encrypt
              </Link>
              <Link to="/decrypt" className="px-3 py-2 rounded-md text-sm font-medium hover:bg-gray-700">
                Decrypt
              </Link>
              <Link to="/admin" className="px-3 py-2 rounded-md text-sm font-medium hover:bg-gray-700">
                Admin
              </Link>
            </div>
          </div>
        </div>
      </nav>

      {/* Main Content */}
      <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
        <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/encrypt" element={<Encrypt />} />
          <Route path="/decrypt" element={<Decrypt />} />
          <Route path="/admin" element={<AdminDashboard />} />
        </Routes>
      </main>

      {/* Footer */}
      <footer className="bg-gray-800 border-t border-gray-700 mt-auto">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-4">
          <p className="text-center text-gray-400 text-sm">
            Honey Encryption - Security through deception
          </p>
        </div>
      </footer>
    </div>
  )
}

export default App

