namespace detail
{
    template<typename T, bool IsFloat = std::is_floating_point<T>::value>
    struct distribution  
    {
        typedef std::uniform_int_distribution<T> dist;
    };
    template<typename T>
    struct distribution<T, true>
    {
        typedef std::uniform_real_distribution<T> dist;
    };
}

//////////////////////////////////////////////

template<typename T>
T Randomizer::range(const T start, const T end) const
{
    return typename detail::distribution<T>::dist(start, end)(m_randomGenerator);
}

//////////////////////////////////////////////

template<typename T>
T Randomizer::operator ()(const T start, const T end) const
{
    return range(start, end);
}

//////////////////////////////////////////////

template<typename T>
glm::vec2 Randomizer::insideCircle(const T radius) const
{
    return glm::normalize(glm::vec2(range(-1.0f, 1.0f), range(-1.0f, 1.0f))) * (std::sqrt(range(0.f, 1.f)) * radius);
}

//////////////////////////////////////////////

template<typename T>
glm::vec3 Randomizer::insideSphere(const T radius) const
{
    return glm::normalize(glm::vec3(range(-1.0f, 1.0f), range(-1.0f, 1.0f), range(-1.0f, 1.0f))) * (std::sqrt(range(0.f, 1.f)) * radius);
}