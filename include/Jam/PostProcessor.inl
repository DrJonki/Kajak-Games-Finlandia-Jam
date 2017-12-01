template<typename T, typename ... Args>
T& PostProcessor::createEffect(const std::string& name, Args&&... args)
{
  auto ptr = std::make_unique<T>(m_instance, name, args...);
  auto& ref = *ptr;

  m_effects.emplace_back(std::move(ptr));
  return ref;
}
