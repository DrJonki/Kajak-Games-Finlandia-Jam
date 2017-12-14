#pragma once

#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include <random>
#include <type_traits>

namespace jam
{
  class Randomizer
  {
  public:

    /// \brief Default constructor
    ///
    /// Initializes the seed using std::random_device.
    ///
    Randomizer();

    /// \brief Constructor for initialization with a custom seed
    ///
    /// \param seed The seed to use
    ///
    Randomizer(const std::default_random_engine::result_type seed);


    /// \brief Returns a random value from given distribution
    ///
    /// \param start Starting point of distribution
    /// \param end Ending point of distribution
    ///
    template<typename T>
    T range(const T start, const T end) const;

    /// \copydoc range()
    ///
    template<typename T>
    T operator ()(const T start, const T end) const;

    /// \brief Get a random point within a circle
    ///
    /// \param radius Radius of the circle
    ///
    /// \return Random point inside a circle
    ///
    template<typename T>
    glm::vec2 insideCircle(const T radius) const;

    /// \brief Get a random point within a sphere
    ///
    /// \param radius Radius of the circle
    ///
    /// \return Random point inside a sphere
    ///
    template<typename T>
    glm::vec3 insideSphere(const T radius) const;

  private:

    mutable std::default_random_engine m_randomGenerator;   ///< The random engine
  };

  // Include the template implementation file
  #include <Jam/Randomizer.inl>
}