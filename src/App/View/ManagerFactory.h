#include <utility>
#include <memory>
#include <QString>
#include <map>
#include <stdexcept>
#include <QDebug>

// Manager base
#include "Managers/Manager.h"


/**
 * View Factory
 *
 */
namespace App { namespace View
{

    class ManagerFactory
    {
        public:

            ManagerFactory()
            {

            }

            ~ManagerFactory()
            {

            }

            ManagerFactory(const ManagerFactory&) = delete;

            /**
             * Create a new sensor instance
             *
             * @author Sam Mottley
             */
            template<typename T, typename... Args>
            QString create(QString view, Args... args)
            {
                // Create sensors as specified
                m_managers[view] = std::make_unique<T>(args...);

                // Return the instance referance
                return view;
            }


            /**
             * Destroy an instance of an sensor
             *
             * @author Sam Mottley
             */
            void destroy()
            {
                if(m_managers.empty())
                    return;

                // Erase the entry
                for(auto& i : m_managers)
                {
                    m_managers.erase(i.first);
                }
            }
            void destroy(QString view)
            {
                // Check exists
                exists(view);

                // Erase the entry
                m_managers.erase(view);
            }


            /**
             * Return a sensors object
             *
             * @author Sam Mottley
             * @param Type type supported sensor type
             * @param int i id of the sensor
             * @return T sensor class
             */
            template<typename T>
            T* get(QString view)
            {
                // Check exists
                exists(view);

                // Turn sensor
                Managers::Manager* tempBase = m_managers[view].get();
                return static_cast<T*>(tempBase);
            }

        private:
            // Container for all our sensors
            std::map<QString, std::unique_ptr<Managers::Manager>> m_managers;

            /**
             * Check ID exists
             *
             * @author Sam Mottley
             */
            void exists(QString view)
            {
                if (m_managers.find(view) == m_managers.end())
                {
                    throw std::range_error("View does not exists");
                }
            }

    };

}}
