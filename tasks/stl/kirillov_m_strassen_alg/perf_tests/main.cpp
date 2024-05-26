// Copyright 2024 Kirillov Maxim
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "stl/kirillov_m_strassen_alg/include/ops_stl.hpp"
using namespace kirillov_stl;

TEST(kirillov_m_strassen_stl_perf_tests, test_pipeline_run) {
  const int n = 64;

  // Create data
  std::vector<double> A = generateRandomMatrix(n);
  std::vector<double> B = generateRandomMatrix(n);
  std::vector<double> out(n * n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskDataSeq->inputs_count.emplace_back(A.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskDataSeq->inputs_count.emplace_back(B.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(const_cast<int *>(&n)));

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  std::vector<double> res = mul(A, B, n);
  // Create Task
  auto strassenMatrixMultParallelStl = std::make_shared<StrassenMatrixMultParallelSTL>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(strassenMatrixMultParallelStl);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < res.size(); i++) {
    EXPECT_NEAR(res[i], out[i], 10e-6);
  }
}

TEST(kirillov_m_strassen_stl_perf_tests, test_task_run) {
  const int n = 64;

  // Create data
  std::vector<double> A = generateRandomMatrix(n);
  std::vector<double> B = generateRandomMatrix(n);
  std::vector<double> out(n * n);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(A.data()));
  taskDataSeq->inputs_count.emplace_back(A.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(B.data()));
  taskDataSeq->inputs_count.emplace_back(B.size());

  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(const_cast<int *>(&n)));

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  std::vector<double> res = mul(A, B, n);
  // Create Task
  auto strassenMatrixMultParallelStl = std::make_shared<StrassenMatrixMultParallelSTL>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(strassenMatrixMultParallelStl);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  for (size_t i = 0; i < res.size(); i++) {
    EXPECT_NEAR(res[i], out[i], 10e-6);
  }
}
