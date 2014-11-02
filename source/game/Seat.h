/*
 *  Copyright (C) 2011-2014  OpenDungeons Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SEAT_H
#define SEAT_H


#include <OgreColourValue.h>
#include <string>
#include <vector>
#include <ostream>
#include <istream>

class Goal;
class ODPacket;

class Seat
{
public:
    friend class GameMap;
    friend class ODClient;
    // Constructors
    Seat();

    //! \brief Adds a goal to the vector of goals which must be completed by this seat before it can be declared a winner.
    void addGoal(Goal* g);

    /** \brief A simple accessor function to return the number of goals
     * which must be completed by this seat before it can be declared a winner.
     */
    unsigned int numUncompleteGoals();

    /** \brief A simple accessor function to allow for looping over the goals
     * which must be completed by this seat before it can be declared a winner.
     */
    Goal* getUncompleteGoal(unsigned int index);

    //! \brief A simple mutator to clear the vector of unmet goals.
    void clearUncompleteGoals();

    //! \brief A simple mutator to clear the vector of met goals.
    void clearCompletedGoals();

    /** \brief Loop over the vector of unmet goals and call the isMet() and isFailed() functions on
     * each one, if it is met move it to the completedGoals vector.
     */
    unsigned int checkAllGoals();

    /** \brief Loop over the vector of met goals and call the isUnmet() function on each one,
     * if any of them are no longer satisfied move them back to the goals vector.
     */
    unsigned int checkAllCompletedGoals();

    //! \brief A simple accessor function to return the number of goals completed by this seat.
    unsigned int numCompletedGoals();

    //! \brief A simple accessor function to allow for looping over the goals completed by this seat.
    Goal* getCompletedGoal(unsigned int index);

    //! \brief A simple accessor function to return the number of goals failed by this seat.
    unsigned int numFailedGoals();

    //! \brief A simple accessor function to allow for looping over the goals failed by this seat.
    Goal* getFailedGoal(unsigned int index);

    unsigned int getNumClaimedTiles();
    void setNumClaimedTiles(const unsigned int& num);

    //! \brief Increment the number of claimed tiles by 1.
    void incrementNumClaimedTiles();

    /** \brief See if the goals has changed since we last checked.
     *  For use with the goal window, to avoid having to update it on every frame.
     */
    bool getHasGoalsChanged();

    void resetGoalsChanged();

    void refreshFromSeat(Seat* s);

    int getTeamId() const
    { return mTeamId; }

    int getId() const
    { return mId; }

    const std::string& getFaction() const
    { return mFaction; }

    const std::string& getColorId() const
    { return mColorId; }

    const Ogre::ColourValue& getColorValue() const
    { return mColorValue; }

    inline int getGold() const
    { return mGold; }

    inline int getGoldMined() const
    { return mGoldMined; }

    inline double getMana() const
    { return mMana; }

    inline double getManaDelta() const
    { return mManaDelta; }

    void resetSpawnPool()
    { mSpawnPool.clear(); }

    void addSpawnableCreature(const std::string& creature_name)
    { mSpawnPool.push_back(creature_name); }

    const std::vector<std::string>& getSpawnPool() const
    { return mSpawnPool; }

    //! \brief Returns true if the given seat is allied. False otherwise
    bool isAlliedSeat(Seat *seat);

    //! \brief Checks if the seat is allowed to do corresponding action
    bool canOwnedCreatureBePickedUpBy(Seat* seat);
    bool canOwnedTileBeClaimedBy(Seat* seat);
    bool canOwnedCreatureUseRoomFrom(Seat* seat);
    bool canRoomBeDestroyedBy(Seat* seat);
    bool canTrapBeDestroyedBy(Seat* seat);

    // TODO : most of these should be private
    //! \brief The team id of the player sitting in this seat.
    int mTeamId;

    //! \brief The name of the faction that this seat is playing as.
    std::string mFaction;

    //! \brief The starting camera location (in tile coordinates) of this seat.
    int mStartingX;
    int mStartingY;

    //! \brief The amount of 'keeper mana' the player has.
    double mMana;

    //! \brief The amount of 'keeper mana' the player gains/loses per turn, updated in GameMap::doTurn().
    double mManaDelta;

    //! \brief The amount of 'keeper HP' the player has.
    double mHp;

    //! \brief The total amount of gold coins mined by workers under this seat's control.
    int mGoldMined;

    int mNumCreaturesControlled;

    int mStartingGold;

    static bool sortForMapSave(Seat* s1, Seat* s2);

    static std::string getFormat();
    friend ODPacket& operator<<(ODPacket& os, Seat *s);
    friend ODPacket& operator>>(ODPacket& is, Seat *s);
    friend std::ostream& operator<<(std::ostream& os, Seat *s);
    friend std::istream& operator>>(std::istream& is, Seat *s);

    static void loadFromLine(const std::string& line, Seat *s);

private:
    void goalsHasChanged();

    //! \brief The actual color that this color index translates into.
    std::string mColorId;
    Ogre::ColourValue mColorValue;

    //! \brief Currently unmet goals, the first Seat to empty this wins.
    std::vector<Goal*> mUncompleteGoals;

    //! \brief Currently met goals.
    std::vector<Goal*> mCompletedGoals;

    //! \brief Currently failed goals which cannot possibly be met in the future.
    std::vector<Goal*> mFailedGoals;

    //! \brief The creatures the current seat is allowed to spawn (when following the conditions)
    std::vector<std::string> mSpawnPool;

    //! \brief How many tiles have been claimed by this seat, updated in GameMap::doTurn().
    unsigned int mNumClaimedTiles;

    bool mHasGoalsChanged;

    //! \brief The total amount of gold coins in the keeper's treasury and in the dungeon heart.
    int mGold;

    //! \brief The seat id. Allows to identify this seat. Must be unique per level file.
    int mId;
};

#endif // SEAT_H