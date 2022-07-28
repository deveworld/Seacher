#pragma once
#include <thread>
#include <algorithm>
#include <random>
#include "Math.hpp"

const int ARRANGE_STATE_INIT            = 1000;
const int ARRANGE_STATE_STOP            = 1001;
const int ARRANGE_STATE_START           = 1002;
const int ARRANGE_STATE_STOPPED         = 1003;

extern void setName(int deskId, std::string name);

class Arranger
{
private:
    int state = ARRANGE_STATE_INIT;
    int epoch = 0;
    int gens = 10;
    std::vector<Desk>* desks;
    std::vector<std::vector<DeskForCal>> geneDesks;
    std::vector<std::string>* names;
    std::thread arrangerThread;
    std::default_random_engine rng = std::default_random_engine {};
    std::vector<DeskForCal>* bestDesk = nullptr;
public:
    Arranger(std::vector<Desk>* p_desks, std::vector<std::string>* p_names);
    void geneDesksInit();
    void start();
    void stop();
    void arrangeLoop();
    void arrange();
    int evaluate(std::vector<DeskForCal> p_desks);
};

Arranger::Arranger(std::vector<Desk>* p_desks, std::vector<std::string>* p_names)
: desks(p_desks)
, names(p_names)
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
        else {
            state = ARRANGE_STATE_STOPPED;
        }
    }
}

void Arranger::arrange()
{
    if (epoch == 0)
    {
        std::vector<std::string> copyNames = *names;

        for (std::vector<DeskForCal> geneDesk : geneDesks)
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
    else {
        int bestScore = -100;
        for (std::vector<DeskForCal> geneDesk : geneDesks)
        {
            int score = evaluate(geneDesk);
            if (score > bestScore)
            {
                bestScore = score;
                bestDesk = &geneDesk;
            }
        }
        // for (DeskForCal desk : *bestDesk)
        size_t i = 0;
        for (Desk& desk : *desks)
        {
            std::string name = bestDesk->at(i).getName();
            desk.setName(name);
            i++;
        }

        std::vector<DeskForCal> presetDesks;
        for (Desk desk : *desks)
        {
            presetDesks.push_back(DeskForCal(desk.getJustCoord(), desk.getName(), desk.isDisabled()));
        }
        std::mt19937 mersenne(rng());
        std::uniform_int_distribution<> random(0, presetDesks.size()-1);

        geneDesks.clear();
        for (std::vector<DeskForCal> geneDesk : geneDesks)
        {
            geneDesk = presetDesks;
            std::string buf;
            int from = random(mersenne);
            int to;
            while (to != from)
            {
                to = random(mersenne);
            }
            
            buf = presetDesks[from].getName();
            presetDesks[from].setName(presetDesks[to].getName());
            presetDesks[to].setName(buf);
        }
    }
    epoch++;
    std::cout << epoch << std::endl;
}

int Arranger::evaluate(std::vector<DeskForCal> p_desks)
{
    return 0;
}