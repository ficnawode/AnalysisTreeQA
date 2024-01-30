
#include "AnalysisTree/SimpleCut.hpp"
#include "AnalysisTree/TaskManager.hpp"
#include "AnalysisTree/Variable.hpp"
#include "Task.hpp"
#include <string>

using namespace AnalysisTree;

SimpleCut CreateEqualsVectorCut(std::string var_name, std::vector<double> values) {
  auto lambda = [values](std::vector<double>& var) -> bool {
    return (
        std::find(std::begin(values), std::end(values), var[0]) != std::end(values));
  };
  return SimpleCut({var_name}, lambda);
}

void pid_gauss(const std::string filelist) {
  auto* man = TaskManager::GetInstance();

  auto* task = new QA::Task;
  task->SetOutputFileName("tof_plots.root");

  Variable p_over_q = Variable("pq", {{"VtxTracks", "p"}, {"VtxTracks", "q"}}, [](std::vector<double>& var) { return var.at(0) / var.at(1); });
  auto mass2 = Variable::FromString("TofHits.mass2");
  mass2.SetName("m2");

  task->AddH2({"t, (ps)", {"TofHits", "t"}, {400, 20, 60}}, {"m^{2}, (GeV^{2}/c^{4})", mass2, {400, -1, 2}});
  task->AddH2({"p, (GeV/c)", p_over_q, {400, 0, 12}}, {"t (ps)", {"TofHits", "t"}, {400, 20, 60}});

  {
    int pdg = 2212;
    auto simpdg_cut = EqualsCut("VtxTracks.mc_pdg", pdg);
    auto true_pdg_cut = new AnalysisTree::Cuts("P_pdg", {simpdg_cut});
    task->AddH2({"t, (ps)", {"TofHits", "t"}, {400, 20, 60}}, {"m^{2}, (GeV^{2}/c^{4})", mass2, {400, -1, 2}}, true_pdg_cut);
    task->AddH2({"p, (GeV/c)", p_over_q, {400, 0, 12}}, {"t (ps)", {"TofHits", "t"}, {400, 20, 60}}, true_pdg_cut);
  }

  {
    int pdg = 321;
    auto simpdg_cut = EqualsCut("VtxTracks.mc_pdg", pdg);
    auto true_pdg_cut = new AnalysisTree::Cuts("K_pdg", {simpdg_cut});
    task->AddH2({"t, (ps)", {"TofHits", "t"}, {400, 20, 60}}, {"m^{2}, (GeV^{2}/c^{4})", mass2, {400, -1, 2}}, true_pdg_cut);
    task->AddH2({"p, (GeV/c)", p_over_q, {400, 0, 12}}, {"t (ps)", {"TofHits", "t"}, {400, 20, 60}}, true_pdg_cut);
  }
  {
    std::vector<double> pdg = {-13, 211, -11};
    auto simpdg_cut = CreateEqualsVectorCut("VtxTracks.mc_pdg", pdg);
    auto true_pdg_cut = new AnalysisTree::Cuts("pi_pdg", {simpdg_cut});
    task->AddH2({"t, (ps)", {"TofHits", "t"}, {400, 20, 60}}, {"m^{2}, (GeV^{2}/c^{4})", mass2, {400, -1, 2}}, true_pdg_cut);
    task->AddH2({"p, (GeV/c)", p_over_q, {400, 0, 12}}, {"t (ps)", {"TofHits", "t"}, {400, 20, 60}}, true_pdg_cut);
  }

  man->AddTask(task);

  man->Init({filelist}, {"rTree"});
  man->Run(-1);
  man->Finish();
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "Error! Please use " << std::endl;
    std::cout << " ./example filename" << std::endl;
    exit(EXIT_FAILURE);
  }

  const std::string filename = argv[1];
  pid_gauss(filename);

  return 0;
}