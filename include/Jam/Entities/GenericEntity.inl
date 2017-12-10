template<typename T>
void GenericEntity<T>::draw(sf::RenderTarget& target)
{
  target.draw(*this);
}