#pragma once
#include <thread>
#include <algorithm>
#include <random>
#include "Math.hpp"
#include "Desk.hpp"
#include <unistd.h>

const int ARRANGE_STATE_INIT            = 1000;
const int ARRANGE_STATE_STOP            = 1001;
const int ARRANGE_STATE_START           = 1002;
const int ARRANGE_STATE_STOPPED         = 1003;

#include <chrono>
#include <cstdint>

uint64_t getTime() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

class Arranger
{
private:
    int state = ARRANGE_STATE_INIT;
    int epoch = 0;
    int gens = 10; // > 1
    std::vector<Desk>* desks;
    std::vector<std::vector<DeskForCal>> geneDesks;
    std::vector<std::string>* names;
    std::vector<std::string>* separateNames;
    std::vector<std::string>* frontNames;
    std::thread arrangerThread;
    std::default_random_engine rng = std::default_random_engine {};
    int bestDesk = 0;
public:
    Arranger(
        std::vector<Desk>* p_desks, 
        std::vector<std::string>* p_names, 
        std::vector<std::string>* p_separateNames,
        std::vector<std::string>* p_frontNames
        );
    void geneDesksInit();
    void start();
    void stop();
    void arrangeLoop();
    void arrange();
    int evaluate(std::vector<DeskForCal> p_desks);
};

Arranger::Arranger(
        std::vector<Desk>* p_desks, 
        std::vector<std::string>* p_names, 
        std::vector<std::string>* p_separateNames,
        std::vector<std::string>* p_frontNames
        )
: desks(p_desks)
, names(p_names)
, separateNames(p_separateNames)
, frontNames(p_frontNames)
{}

void Arranger::geneDesksInit()
{
    std::vector<DeskForCal> presetDesks;
    for (Desk desk : *desks)
    {
        presetDesks.push_back(DeskForCal(desk.getJustCoord(), desk.getName(), desk.isDisabled()));
    }
    geneDesks.clear();
    geneDesks = std::vector<std::vector<DeskForCal>>(gens, presetDesks);
}

void Arranger::start()
{
    geneDesksInit();
    if (state == ARRANGE_STATE_INIT)
    {
        arrangerThread = std::thread([&] () { arrangeLoop(); });
        arrangerThread.detach();
    }
    state = ARRANGE_STATE_START;
}

void Arranger::stop()
{
    state = ARRANGE_STATE_STOP;
}

void Arranger::arrangeLoop()
{
    while (true)
    {
        if (state == ARRANGE_STATE_START)
        {
            arrange();
        }
        else if (state == ARRANGE_STATE_STOP) {
            state = ARRANGE_STATE_STOPPED;
            std::cout << "stopped" << std::endl;
        }
    }
}

void Arranger::arrange()
{
    if (epoch == 0)
    {
        std::vector<std::string> copyNames = *names;

        for (std::vector<DeskForCal>& geneDesk : geneDesks)
        {
            std::shuffle(copyNames.begin(), copyNames.end(), rng);
            int i = 0;
            for (DeskForCal& desk : geneDesk)
            {
                if (desk.isDisabled())
                {
                    continue;
                }
                
                desk.setName(copyNames[i]);
                i++;
            }
        }
    }
    else 
    {
        int bestScore = -100;
        size_t i = 0;
        for (std::vector<DeskForCal>& geneDesk : geneDesks)
        {
            int score = evaluate(geneDesk);
            if (score > bestScore)
            {
                bestScore = score;
                bestDesk = i;
            }
            i++;
        }
        // for (DeskForCal desk : *bestDesk)
        i = 0;
        for (Desk& desk : *desks)
        {
            std::string name = geneDesks[bestDesk][i].getName();
            desk.setName(name);
            i++;
        }

        std::vector<DeskForCal> presetDesks;
        for (Desk desk : *desks)
        {
            presetDesks.push_back(DeskForCal(desk.getJustCoord(), desk.getName(), desk.isDisabled()));
        }
        srand(getTime());
        std::mt19937 mersenne(rng());
        std::uniform_int_distribution<> random(0, presetDesks.size()-1);

        geneDesks.clear();
        bool first = true;
        for (int i = 0; i < gens; i++)
        {
            geneDesks.push_back(presetDesks);
            if (!first)
            {
                std::string buf;
                int from = random(mersenne);
                int to = random(mersenne);
                while (to == from)
                {
                    from = random(mersenne);
                    to = random(mersenne);
                }

                buf = geneDesks[i][from].getName();
                geneDesks[i][from].setName(geneDesks[i][to].getName());
                geneDesks[i][to].setName(buf);
            }
            first = false;
        }
    }
    epoch++;
}

int Arranger::evaluate(std::vector<DeskForCal> p_desks)
{
    int score = 0;
    for (DeskForCal desk : p_desks)
    {
        if (std::find(separateNames->begin(), separateNames->end(), desk.getName()) != separateNames->end())
        {
            for (std::string separateName1 : *separateNames)
            {
                if (separateName1 != desk.getName())
                {
                    for (DeskForCal otherDesk : p_desks)
                    {
                        if (otherDesk.getName() == separateName1)
                        {
                            score += abs(desk.getCoord().x - otherDesk.getCoord().x)
                                   + abs(desk.getCoord().y - otherDesk.getCoord().y);
                        }
                    }
                }
            }
        }

        if (std::find(frontNames->begin(), frontNames->end(), desk.getName()) != frontNames->end())
        {
            score += desk.getCoord().y;
        }
    }
    
    return score;
}