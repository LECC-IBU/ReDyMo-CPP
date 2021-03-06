#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../include/chromosome.hpp"
#include "../include/data_manager.hpp"
#include "../include/util.hpp"

class DataManagerTest : public ::testing::Test
{
  protected:
    std::shared_ptr<DataManager> data;

  protected:
    DataManagerTest() {}

    void SetUp()
    {
        data = std::make_shared<DataManager>("dummy", "../data/database.sqlite",
                                             "../data/MFA-Seq_dummy/");
    }

    void TearDown() {}
};

TEST_F(DataManagerTest, GenerateProbLandscape)
{
    // This data is a sample from every 1/7 of the landscape. The dummy data
    // forms discrete "buckets" in the landscape, so we can check by samples.
    std::vector<double> expected_samples = {
        0.00010000000000021103, 0.3334000000000006, 1.0,
        0.6667000000000001,     0.6667000000000001, 0.3334000000000006,
        0.1667500000000004};

    std::vector<double> result = data->get_probability_landscape("dummy_01");
    int step                   = std::ceil(result.size() / 7);

    // This checks near the center of each bucket for the value
    for (int i = 1; i < (int)expected_samples.size(); i++)
        ASSERT_NEAR(result[std::ceil(i * step) - step / 2],
                    expected_samples[i - 1], 1e-10);
}

TEST_F(DataManagerTest, GetTranscriptionRegions)
{
    std::vector<transcription_region_t> result =
        *data->get_transcription_regions("dummy_01");
    ASSERT_EQ(result[0].start, 25);
    ASSERT_EQ(result[0].end, 80);
}

TEST_F(DataManagerTest, GetConstitutiveOrigin)
{
    std::vector<constitutive_origin_t> result =
        *data->get_constitutive_origins("dummy_01");
    ASSERT_EQ(result[0].base, 1234);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
