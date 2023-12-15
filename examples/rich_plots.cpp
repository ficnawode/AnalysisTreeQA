#include <string>

#include "AnalysisTree/SimpleCut.hpp"
#include "AnalysisTree/TaskManager.hpp"
#include "AnalysisTree/Variable.hpp"
#include "Task.hpp"

using namespace AnalysisTree;

void create_rich_plots(QA::Task* task, int pdg, std::string particle_name, double p_min, double p_max, int nbins) {
  Variable p_over_q = Variable("pq", {{"VtxTracks", "p"}, {"VtxTracks", "q"}}, [](std::vector<double>& var) { return var.at(0) / var.at(1); });

  auto simpdg_cut = EqualsCut("VtxTracks.mc_pdg", pdg);

  Cuts* all_simulated_cut = new Cuts(particle_name, {simpdg_cut});

  task->AddH2({"p/q, (GeV/c)", p_over_q, {nbins, p_min, p_max}}, {"r, (cm)", Variable::FromString("RichRings.radius"), {nbins, 0, 10}}, all_simulated_cut);

  task->AddH1({"p/q, (GeV/c)", p_over_q, {nbins, p_min, p_max}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.radius")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.axis_a")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.axis_b")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.center_x")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.center_y")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.chi2_ov_ndf")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.phi")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.phi_ellipse")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.radial_angle")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.radial_pos")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.signal")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.x")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.y")}, {nbins, -1, 10}}, all_simulated_cut);
  task->AddH1({"p/q, (GeV/c)", {Variable::FromString("RichRings.z")}, {nbins, -1, 10}}, all_simulated_cut);
}

void rich_plots(const std::string& filelist) {
  auto* man = TaskManager::GetInstance();

  auto* task = new QA::Task;
  task->SetOutputFileName("rich_plots.root");

  task->AddH1({"pid, []", Variable::FromString("VtxTracks.mc_pdg"), {200, -6, 6}});
  Variable p_over_q = Variable("pq", {{"VtxTracks", "p"}, {"VtxTracks", "q"}}, [](std::vector<double>& var) { return var.at(0) / var.at(1); });
  task->AddH2({"p/q, (GeV/c)", p_over_q, {800, -12, 12}}, {"radius", Variable::FromString("RichRings.radius"), {400, 0, 10}});

  create_rich_plots(task, 11, "electron", -12, 0, 400);
  create_rich_plots(task, -11, "positron", 0, 12, 400);
  create_rich_plots(task, 211, "pi+", 0, 12, 400);
  create_rich_plots(task, -211, "pi-", -12, 0, 400);

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
  rich_plots(filename);

  return 0;
}
