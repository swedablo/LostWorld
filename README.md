# LostWorld

The initial setup of the playground is presented at "DefaultStartGame.txt". Games can be saved and loaded. To win the game a chest needs to be opened, 
containing a portal to a safer world. To open the chest a Magical Key has to be found, hidden within the game.

### Updated Version: ###

- Member functions. Undead, Priest and Warrior are now using member function pointers. 
- Character map. When a character needs to interact with another character (in the environment class) this is done by a std::map<std::string, character*> in the environment.
- Lambda function. In Character::taking_damage a lambda function is used for decreasing dmg if the attacked has resilience.

