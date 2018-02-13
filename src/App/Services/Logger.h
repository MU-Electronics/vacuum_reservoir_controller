#pragma once

namespace App { namespace Services{
    class Logger
    {
    public:
        Logger();

    private:
        explicit Logger(const Logger& rhs) = delete;
        Logger& operator= (const Logger& rhs) = delete;
    };
}}


