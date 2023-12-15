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
  task->SetOutputFileName("b_eng_histos.root");

  Variable p_over_q = Variable("pq", {{"VtxTracks", "p"}, {"VtxTracks", "q"}}, [](std::vector<double>& var) { return var.at(0) / var.at(1); });
  auto mass2 = Variable::FromString("TofHits.mass2");
  mass2.SetName("m2");

  task->AddH2({"p, (GeV/c)", p_over_q, {100, 0, 12}}, {"TRD Energy Loss 0, (GeV)", {"TrdTracks", "energy_loss_0"}, {100, -1, 12}});
  task->AddH2({"p, (GeV/c)", p_over_q, {100, 0, 12}}, {"TRD Energy Loss 1, (GeV)", {"TrdTracks", "energy_loss_1"}, {100, -1, 12}});
  task->AddH2({"p, (GeV/c)", p_over_q, {100, 0, 12}}, {"TRD Energy Loss 2, (GeV)", {"TrdTracks", "energy_loss_2"}, {100, -1, 12}});
  task->AddH2({"p, (GeV/c)", p_over_q, {100, 0, 12}}, {"TRD Energy Loss 3, (GeV)", {"TrdTracks", "energy_loss_3"}, {100, -1, 12}});

  task->AddH2({"#eta ", {"VtxTracks", "eta"}, {400, 0, 12}}, {"p_T, (GeV/c) ", {"VtxTracks", "pT"}, {400, -0.5, 4}});

  task->AddH2({"p/q, (GeV/c)", p_over_q, {400, 0, 12}}, {"m^{2}, (GeV^{2}/c^{4})", mass2, {400, -1, 2}});

  {
    int pdg = 2212;
    auto simpdg_cut = EqualsCut("VtxTracks.mc_pdg", pdg);
    auto true_pdg_cut = new AnalysisTree::Cuts("P_pdg", {simpdg_cut});
    task->AddH2({"p/q, (GeV/c)", p_over_q, {400, 0, 12}}, {"m^{2}, (GeV^{2}/c^{4})", mass2, {400, -1, 2}}, true_pdg_cut);
    task->AddH2({"#eta ", {"VtxTracks", "eta"}, {400, 0, 8}}, {"p_T, (GeV/c) ", {"VtxTracks", "pT"}, {400, -0.1, 4}}, true_pdg_cut);
    task->AddH2({"p, (GeV/c)", p_over_q, {100, 0, 12}}, {"TRD Energy Loss 0, (GeV)", {"TrdTracks", "energy_loss_0"}, {100, 0.1, 12}}, true_pdg_cut);
  }

  {
    int pdg = 321;
    auto simpdg_cut = EqualsCut("VtxTracks.mc_pdg", pdg);
    auto true_pdg_cut = new AnalysisTree::Cuts("K_pdg", {simpdg_cut});
    task->AddH2({"p/q, (GeV/c)", p_over_q, {400, 0, 12}}, {"m^{2}, (GeV^{2}/c^{4})", mass2, {400, -1, 2}}, true_pdg_cut);
    task->AddH2({"#eta ", {"VtxTracks", "eta"}, {400, 0, 8}}, {"p_T, (GeV/c) ", {"VtxTracks", "pT"}, {400, -0.1, 4}}, true_pdg_cut);
    task->AddH2({"p, (GeV/c)", p_over_q, {100, 0, 12}}, {"TRD Energy Loss 0, (GeV)", {"TrdTracks", "energy_loss_0"}, {100, 0.1, 12}}, true_pdg_cut);
  }
  {
    std::vector<double> pdg = {-13, 211, -11};
    auto simpdg_cut = CreateEqualsVectorCut("VtxTracks.mc_pdg", pdg);
    auto true_pdg_cut = new AnalysisTree::Cuts("pi_pdg", {simpdg_cut});
    task->AddH2({"p/q, (GeV/c)", p_over_q, {400, 0, 12}}, {"m^{2}, (GeV^{2}/c^{4})", mass2, {400, -1, 2}}, true_pdg_cut);
    task->AddH2({"#eta ", {"VtxTracks", "eta"}, {400, 0, 8}}, {"p_T, (GeV/c) ", {"VtxTracks", "pT"}, {400, -0.1, 4}}, true_pdg_cut);
    task->AddH2({"p, (GeV/c)", p_over_q, {100, 0, 12}}, {"TRD Energy Loss 0, (GeV)", {"TrdTracks", "energy_loss_0"}, {100, 0.1, 12}}, true_pdg_cut);
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
