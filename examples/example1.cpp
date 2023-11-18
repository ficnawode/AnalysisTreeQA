#include <string>

#include "AnalysisTree/SimpleCut.hpp"
#include "AnalysisTree/TaskManager.hpp"
#include "AnalysisTree/Variable.hpp"
#include "Task.hpp"

using namespace AnalysisTree;

SimpleCut CreateNotEqualsCut(std::string var_name, const double value) {
  auto lambda = [value](std::vector<double>& var) -> bool {
    return std::fabs(value - var.at(0)) > 0.1;
  };
  return SimpleCut({var_name}, lambda);
}

SimpleCut CreateNotEqualsVectorCut(std::string var_name, std::vector<double> values) {
  auto lambda = [values](std::vector<double>& var) -> bool {
    return (
        std::find(std::begin(values), std::end(values), var[0]) == std::end(values));
  };
  return SimpleCut({var_name}, lambda);
}

SimpleCut CreateEqualsVectorCut(std::string var_name, std::vector<double> values) {
  auto lambda = [values](std::vector<double>& var) -> bool {
    return (
        std::find(std::begin(values), std::end(values), var[0]) != std::end(values));
  };
  return SimpleCut({var_name}, lambda);
}

void example(const std::string& filelist) {
  auto* man = TaskManager::GetInstance();

  auto* task = new QA::Task;
  task->SetOutputFileName("onnx_qa.root");

  std::array<std::string, 3> particle_names = {"P", "K+", "Pi+"};

  for (int j = 0; j < 3; j++) {
    static constexpr int nbdtbins = 1000;
    std::string j_str = std::to_string(j);
    auto hist_title = "BDT " + particle_names[j] + " , %";
    auto bdt_var_name = "VtxTracks.onnx_pred_0_" + j_str;
    auto bdt_var = Variable::FromString(bdt_var_name);
    bdt_var.SetName("BDT_" + particle_names[j] + '_' + std::to_string(j));
    task->AddH1({hist_title, bdt_var, {nbdtbins, 0, 1}});
  }

  task->AddH1({"momentum p, []", Variable::FromString("VtxTracks.p"), {200, -6, 6}});
  task->AddH1({"pid, []", Variable::FromString("VtxTracks.mc_pdg"), {200, -6, 6}});

  Variable p_over_q = Variable("pq", {{"VtxTracks", "p"}, {"VtxTracks", "q"}}, [](std::vector<double>& var) { return var.at(0) / var.at(1); });
  auto mass2 = Variable::FromString("TofHits.mass2");

  mass2.SetName("m2");
  task->AddH2({"p/q, GeV/c", p_over_q, {400, -12, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}});

  auto q_cut = RangeCut("VtxTracks.q", 0, 2);

  // PROTONS
  {
    auto bdt_cut = RangeCut("VtxTracks.onnx_pred_0_0", 0.999, 1.1);
    int pdg = 2212;
    auto simpdg_cut = EqualsCut("VtxTracks.mc_pdg", pdg);
    auto wrong_pid_cut = CreateNotEqualsCut("VtxTracks.mc_pdg", pdg);

    Cuts* xgb_cut = new Cuts("P_xgb", {bdt_cut, q_cut});
    Cuts* all_simulated_cut = new Cuts("P_all_simulated", {simpdg_cut});
    Cuts* match_cut = new Cuts("P_match", {bdt_cut, q_cut, simpdg_cut});
    Cuts* mismatch_cut = new Cuts("P_mismatch", {bdt_cut, q_cut, wrong_pid_cut});

    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, xgb_cut);
    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, all_simulated_cut);
    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, match_cut);
    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, mismatch_cut);

    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, xgb_cut);
    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, all_simulated_cut);
    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, match_cut);
    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, mismatch_cut);
  }

  // kaons
  {
    auto bdt_cut = RangeCut("VtxTracks.onnx_pred_0_1", 0.999, 1.1);
    int pdg = 321;
    auto simpdg_cut = EqualsCut("VtxTracks.mc_pdg", pdg);
    auto wrong_pid_cut = CreateNotEqualsCut("VtxTracks.mc_pdg", pdg);

    Cuts* xgb_cut = new Cuts("K+_xgb", {bdt_cut, q_cut});
    Cuts* all_simulated_cut = new Cuts("K+_all_simulated", {simpdg_cut});
    Cuts* match_cut = new Cuts("K+_match", {bdt_cut, q_cut, simpdg_cut});
    Cuts* mismatch_cut = new Cuts("K+_mismatch", {bdt_cut, q_cut, wrong_pid_cut});

    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, xgb_cut);
    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, all_simulated_cut);
    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, match_cut);
    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, mismatch_cut);

    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, xgb_cut);
    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, all_simulated_cut);
    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, match_cut);
    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, mismatch_cut);
  }

  // PROTONS
  {
    auto bdt_cut = RangeCut("VtxTracks.onnx_pred_0_2", 0.999, 1.1);
    std::vector<double> pdg{13, 211, 11};
    auto simpdg_cut = CreateEqualsVectorCut("VtxTracks.mc_pdg", pdg);
    auto wrong_pid_cut = CreateNotEqualsVectorCut("VtxTracks.mc_pdg", pdg);

    Cuts* xgb_cut = new Cuts("Pi+_xgb", {bdt_cut, q_cut});
    Cuts* all_simulated_cut = new Cuts("Pi+_all_simulated", {simpdg_cut});
    Cuts* match_cut = new Cuts("Pi+_match", {bdt_cut, q_cut, simpdg_cut});
    Cuts* mismatch_cut = new Cuts("Pi+_mismatch", {bdt_cut, q_cut, wrong_pid_cut});

    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, xgb_cut);
    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, all_simulated_cut);
    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, match_cut);
    task->AddH2({"p/q, GeV/c", p_over_q, {400, 0, 12}}, {"m^{2}, GeV^{2}/c^{4}", mass2, {400, -1, 2}}, mismatch_cut);

    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, xgb_cut);
    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, all_simulated_cut);
    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, match_cut);
    task->AddH1({"p/q, GeV/c", p_over_q, {400, 0, 12}}, mismatch_cut);
  }

  // TODO: add plots for where the bdt cuts do mismatches (simparticles.pdg)
  // TODO: add plots for where the bdt cuts are right (simparticles.pdg)
  // TODO: add plots for where the actual protons, k and pi are (simparticles.pdg)

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
  example(filename);

  return 0;
}
