#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <TGraph.h>
#include <TCanvas.h>

double max_limit;
double min_limit;


std::pair<std::vector<std::vector<double>>, std::vector<double>> read(std::ifstream& input_file, int number_of_bands, int number_of_columns){
    
    std::vector<std::vector<double>> bands(number_of_bands); 
    std::vector<double> coordinates;

    for (int i = 0; i < number_of_columns; i++)
    {
        coordinates.push_back(i);
    }

    double d;
    std::vector<double> temp;
    while (input_file >> d)
    {
        temp.push_back(d);
    }
    
    for (int column = 0; column < number_of_columns; column++)
    {
        for (int line = 0; line < number_of_bands + 3; line++)
    {
        if (line - 3 >= 0)
        {
            bands[line-3].push_back(temp[column*(3 + number_of_bands) + line]);
        }
    }
    }

    max_limit = *std::max_element(temp.begin(), temp.end());
    min_limit = *std::min_element(temp.begin(), temp.end());

    temp.clear();

    return {bands, coordinates};
}

auto fill_graphs(std::vector<std::vector<double>>& bands, std::vector<double>& coordinates){
    std::vector<TGraph*> graphs(bands.size());

    for (int i = 0; i < bands.size(); i++)
    {
        graphs[i] = new TGraph{static_cast<int>(coordinates.size())};

        for (int j = 0; j < bands[i].size(); j++)
        {
            graphs[i]->SetPoint(j, coordinates[j], bands[i][j]);
        }
        
    }
    
    return graphs;
}

void print_graphs(std::vector<TGraph*>& graphs){
    std::vector<TCanvas*> canvass(graphs.size());
    for (int i = 0; i < graphs.size(); i++)
    {
        canvass[i] = new TCanvas{Form("Band_%d", i+1), Form("Band %d", i+1), 800, 600};
        canvass[i]->cd();
        graphs[i]->Draw("ALP");
        canvass[i]->Print(Form("Band_%d.pdf", i+1));
    }

    TCanvas* Total = new TCanvas{"Bands", "Bands", 800,600};
    Total->cd();
    graphs[0]->SetMaximum(max_limit);
    graphs[0]->SetMinimum(min_limit);
    graphs[0]->SetTitle("Bands");
    graphs[0]->Draw("ALP");
    for (int i = 1; i < graphs.size(); i++)
    {
        graphs[i]->Draw("LP SAME");
    }
    Total->Update();
    Total->Print("Bands.pdf");
    

    for (auto* graph : graphs) {
        delete graph;
    }
    graphs.clear();

    for (auto* canvas : canvass) {
        delete canvas;
    }
    canvass.clear();
}

void clear_vectors(std::vector<std::vector<double>>& bands, std::vector<double>& coordinates){
    for (auto& band : bands) {
        band.clear();
    }
    bands.clear();
    coordinates.clear();
}

void cosmetics(std::vector<TGraph*>& graphs){
    int i = 1;
    for (auto* graph : graphs)
    {
        graph->SetTitle(Form("Band %d; K points; Energy (Ev)",i));
        graph->SetMarkerColor(i);
        graph->SetLineColor(i);
        i++;
    }
    
}

int main(){
    std::cout << "Input file name:" << '\n';
    std::string input_file;
    std::cin >> input_file;
    std::ifstream inp{input_file};

    std::cout << "Select number of bands to plot:" <<'\n';
    int n_bands;
    std::cin >> n_bands;
    std::cout << "Select number of K-points used:" <<'\n';
    int n_points;
    std::cin >> n_points;

    std::cout << "Printing graphs..." << '\n';
    auto [bands, coordinates] = read(inp, n_bands, n_points);
    auto graphs = fill_graphs(bands, coordinates);
    cosmetics(graphs);
    print_graphs(graphs);



    std::cout << "Graphs printed!" << '\n' << "Clearing data..." << '\n';
    clear_vectors(bands, coordinates);
    std::cout << "Data cleared!" << '\n';
}