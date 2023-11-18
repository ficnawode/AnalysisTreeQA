#include "AnalysisTree/SimpleCut.hpp"
#include "AnalysisTree/TaskManager.hpp"
#include "AnalysisTree/Variable.hpp"
#include "Task.hpp"
#include <string>

using namespace AnalysisTree;

void pid_gauss(const std::string filelist) {
  auto* man = TaskManager::GetInstance();

  auto* task = new QA::Task;
  task->SetOutputFileName("pid_gauss.root");

  Variable p_over_q = Variable("pq", {{"VtxTracks", "p"}, {"VtxTracks", "q"}}, [](std::vector<double>& var) { return var.at(0) / var.at(1); });
  auto mass2 = Variable::FromString("TofHits.mass2");
  mass2.SetName("m2");

  {
    int pdg = 2212;
    auto simpdg_cut = EqualsCut("VtxTracks.mc_pdg", pdg);
    auto true_pdg_cut = new AnalysisTree::Cuts("P_pdg", {simpdg_cut});
    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, true_pdg_cut);

    auto p_cut = RangeCut("VtxTracks.p", 2, 2.25);
    auto bin_cut = new AnalysisTree::Cuts("P_pdg", {simpdg_cut, p_cut});
    task->AddH1({"m^2, GeV^{2}/c^{4}",
                 Variable::FromString("TofHits.mass2"),
                 {1000, -1, 2}},
                bin_cut);
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