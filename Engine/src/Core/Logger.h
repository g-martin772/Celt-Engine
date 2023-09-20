#pragma once
#include <fstream>
#include <mutex>
#include <queue>

namespace CeltEngine
{
    enum class LogLevel {
        FATAL,
        ERROR,
        WARNING,
        INFO,
        DEBUG,
        TRACE
    };
    
    class Logger
    {
    public:
        static void Init(const std::string& filename);
        static void Shutdown();

        void Log(LogLevel level, std::string_view message);
        static Logger* GetCoreLogger();
    private:
        Logger(const std::string& filename);
        ~Logger();

        void LogConsumer();
    private:
        std::ofstream m_LogFile;
        std::mutex m_LogMutex;
        std::queue<std::pair<LogLevel, std::string_view>> m_LogQueue;
        std::condition_variable m_Condition;
        bool m_Running;
        std::thread m_ConsumerThread;
    };
}

#define CE_FATAL(message, ...) CeltEngine::Logger::GetCoreLogger()->Log(CeltEngine::LogLevel::FATAL, message)
#define CE_ERROR(message, ...) CeltEngine::Logger::GetCoreLogger()->Log(CeltEngine::LogLevel::ERROR, message)

#if defined(CE_DEBUG) || defined(CE_RELEASE)
#define CE_WARN(message, ...) CeltEngine::Logger::GetCoreLogger()->Log(CeltEngine::LogLevel::WARN, message)
#define CE_INFO(message, ...) CeltEngine::Logger::GetCoreLogger()->Log(CeltEngine::LogLevel::INFO, message)
#endif

#ifdef CE_DEBUG
#define CE_LOG_DEBUG_MESSAGE(message, ...) CeltEngine::Logger::GetCoreLogger()->Log(CeltEngine::LogLevel::DEBUG, message)
#define CE_TRACE(message, ...) CeltEngine::Logger::GetCoreLogger()->Log(CeltEngine::LogLevel::TRACE, message)
#endif
