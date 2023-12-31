﻿#include "cepch.h"
#include "Logger.h"

#include <cstdarg>

namespace CeltEngine
{
    static Logger* s_CoreLogger;
    
    std::string LogLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::FATAL: return "FATAL";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::WARN: return "WARNING";
            case LogLevel::INFO: return "INFO";
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::TRACE: return "TRACE";
            default: return "UNKNOWN";
        }
    }

    void Logger::Init(const std::string& filename)
    {
        s_CoreLogger = new Logger(filename);
    }

    void Logger::Shutdown()
    {
        delete s_CoreLogger;
        s_CoreLogger = nullptr;
    }

    Logger::Logger(const std::string& filename): m_Running(true)
    {
        m_LogFile.open(filename, std::ios::out | std::ios::app);
        m_ConsumerThread = std::thread(&Logger::LogConsumer, this);

        if(!m_LogFile)
        {
            printf("Failed to open log file");
        }
    }

    Logger::~Logger()
    {
        m_Running = false;
        m_Condition.notify_all();
        m_ConsumerThread.join();
        m_LogFile.close();
    }

    void Logger::Log(LogLevel level, const char* message, ...)
    {
        std::lock_guard<std::mutex> lock(m_LogMutex);
        char buffer[8000] = {};

        va_list args; 
        va_start(args, message);

        const int result = vsnprintf(buffer, sizeof(buffer), message, args);

        va_end(args);
        
        if (!(result >= 0 && static_cast<std::size_t>(result) < sizeof(buffer))) {
            std::cout << "Formatting error or buffer too small." << std::endl;
        }

        std::string formattedString(buffer);
        m_LogQueue.push(std::make_pair(level, formattedString));
        m_Condition.notify_one();
    }

    void Logger::LogConsumer()
    {
        while (m_Running) {
            std::unique_lock<std::mutex> lock(m_LogMutex);
            m_Condition.wait(lock, [this] { return !m_LogQueue.empty() || !m_Running; });

            while (!m_LogQueue.empty()) {
                auto logEntry = m_LogQueue.front();
                m_LogQueue.pop();
                lock.unlock();

                // Get current time
                std::time_t now = std::time(nullptr);
                char timeBuffer[64];
                std::strftime(timeBuffer, sizeof(timeBuffer), "[%Y-%m-%d %H:%M:%S]", std::localtime(&now));

                std::stringstream logStream;
                logStream << timeBuffer << " [" << LogLevelToString(logEntry.first) << "]: " << logEntry.second << std::endl;

                // Print to console
                std::cout << logStream.str();

                // Write to the log file
                m_LogFile << logStream.str() << std::endl;

                lock.lock();
            }

            m_LogFile.flush();
        }
    }

    Logger* Logger::GetCoreLogger()
    {
        return s_CoreLogger;
    }
}
