/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2020 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "test/mocks/CapMock.h"
#include <fstream>
#include <json/json.h>
extern "C" {
#include "cap.h"
#include<string.h>
#include<unistd.h>
#include<sys/capability.h>
}

void populate_capabilities(Json::Value cfg_root,
                           std::string caps_list,
                           cap_value_t* appcaps_list,
                           short int* cap_count);
static void reset_cap_user(cap_user *cu)
{
    memset(cu, 0, sizeof(cap_user));
}

using  namespace std;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;
using testing::DoAll;
using testing::SetArgPointee;
using testing::Mock;

CapMock * g_CapMock = NULL;

int add (int num1, int num2)
{
return (num1+num2);
}

TEST(Add, AddCase)
{
EXPECT_EQ(30,add(10,20));
EXPECT_EQ(50,add(30,20));
}

class InitCapTestFixture : public ::testing::Test {
protected:
CapMock mockedCap;

InitCapTestFixture()
{
    g_CapMock = &mockedCap;
    testing::Mock::AllowLeak(&mockedCap);
}
virtual ~InitCapTestFixture()
{
g_CapMock = NULL;
}
virtual void SetUp()
{
printf("%s\n", __func__);
}
virtual void TearDown()
{
printf("%s\n", __func__);
}
static void SetUpTestCase()
        {
                printf("%s\n", __func__);
        }
        static void TearDownTestCase()
        {
                printf("%s\n", __func__);
        }
};
TEST_F(InitCapTestFixture, InitCapTest)
{
        cap_user appcaps;
        cap_t ret_cap = NULL;
        EXPECT_CALL(*g_CapMock, cap_get_proc())
                .Times(1)
                .WillOnce(Return(ret_cap));
        EXPECT_EQ(false, drop_root_priv(&appcaps));
}

TEST_F(InitCapTestFixture, UpdateCapsTest)
{
cap_user appcaps;
appcaps.add_count =1;
appcaps.drop_count =0;
        EXPECT_CALL(*g_CapMock, cap_set_proc(_))
                .Times(1)
                .WillOnce(Return(-1));
EXPECT_CALL(*g_CapMock, cap_set_flag(_,_,_,_,_))
                .Times(2)
                .WillOnce(Return(0))
                .WillOnce(Return(0));
        EXPECT_EQ(-1, update_process_caps(&appcaps));
}


TEST_F(InitCapTestFixture, ReadCapsTest)
{

cap_t ret_cap =(cap_t) 0xffffffff;
char retval[]="test";
cap_user appcaps;
EXPECT_CALL(*g_CapMock, cap_get_pid(_))
                .Times(1)
                .WillOnce(Return(ret_cap));

        EXPECT_CALL(*g_CapMock, cap_to_text(_,_))
                .Times(1)
                .WillOnce(Return(retval));

EXPECT_CALL(*g_CapMock, cap_free(_))
                .Times(2)
                .WillOnce(Return(0))
                .WillOnce(Return(0));

EXPECT_NO_THROW(read_capability(&appcaps));
}

TEST(DROP_PRIV_TEST, UpdateProcCapAdd)
{
            cap_user appcaps;
            appcaps.add_count = -1;
            EXPECT_EQ(-1, update_process_caps(&appcaps));
}

TEST(DROP_PRIV_TEST, UpdateProcCapDrop)
{
            cap_user appcaps;
            appcaps.drop_count = -1;
            EXPECT_EQ(-1, update_process_caps(&appcaps));
}


TEST_F(InitCapTestFixture, IsNonRoot_Positive)
{
    // Non-root user
    EXPECT_CALL(*g_CapMock, getuid())
        .Times(1)
        .WillOnce(Return((uid_t)1000));

    EXPECT_TRUE(isNonroot());
}

TEST_F(InitCapTestFixture, IsNonRoot_Negative)
{
    // Root user
    EXPECT_CALL(*g_CapMock, getuid())
        .Times(1)
        .WillOnce(Return((uid_t)0));

    EXPECT_FALSE(isNonroot());
}

// clear_caps() <E2><80><93> with and without caps string

TEST_F(InitCapTestFixture, ClearCaps_CallsFree)
{
    cap_user appcaps{};
    appcaps.caps = strdup("test_caps");

    EXPECT_CALL(*g_CapMock, cap_free(_))
        .Times(1)
        .WillOnce(Return(0));

    clear_caps(&appcaps);
    EXPECT_EQ(appcaps.caps, nullptr);
}

TEST_F(InitCapTestFixture, ClearCaps_Null)
{
    cap_user appcaps{};
    appcaps.caps = nullptr;

    // Should not call cap_free when caps is already NULL
    EXPECT_CALL(*g_CapMock, cap_free(_)).Times(0);

    clear_caps(&appcaps);
    EXPECT_EQ(appcaps.caps, nullptr);
}

TEST_F(InitCapTestFixture, UpdateProcCaps_AddSuccess)
{
    cap_user cu{};
    cu.add_count = 1;
    cu.drop_count = 0;
    cu.add[0] = CAP_CHOWN;

    // Allow any number of cap_set_flag calls
    EXPECT_CALL(*g_CapMock, cap_set_flag(_,_,_,_,_))
        .Times(::testing::AtLeast(1))
        .WillRepeatedly(Return(0));

    // Allow any ambient capability calls
    EXPECT_CALL(*g_CapMock, cap_get_ambient(_))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*g_CapMock, cap_set_ambient(_, _))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*g_CapMock, cap_set_proc(_))
        .Times(::testing::AtLeast(1))
        .WillRepeatedly(Return(0));

    EXPECT_EQ(update_process_caps(&cu), 0);
}

TEST_F(InitCapTestFixture, UpdateProcCaps_DropSuccess)
{
    cap_user cu{};
    cu.add_count = 0;
    cu.drop_count = 1;
    cu.drop[0] = CAP_DAC_OVERRIDE;

    EXPECT_CALL(*g_CapMock, cap_set_flag(_,_,_,_,_))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*g_CapMock, cap_set_proc(_))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*g_CapMock, cap_get_ambient(_))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*g_CapMock, cap_set_ambient(_, _))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    EXPECT_EQ(update_process_caps(&cu), 0);
}

TEST_F(InitCapTestFixture, UpdateProcCaps_SetFlagFails)
{
    cap_user cu{};
    cu.add_count = 1;
    cu.add[0] = CAP_CHOWN;
    cu.drop_count = 0;

    // First flag fails; other flags succeed
    EXPECT_CALL(*g_CapMock, cap_set_flag(_,_,_,_,_))
        .WillOnce(Return(-1))
        .WillRepeatedly(Return(0));

    // cap_set_proc is still called and succeeds
    EXPECT_CALL(*g_CapMock, cap_set_proc(_))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*g_CapMock, cap_get_ambient(_))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*g_CapMock, cap_set_ambient(_, _))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    // The actual return is 0 (not -1)
    EXPECT_EQ(update_process_caps(&cu), 0);
}

TEST_F(InitCapTestFixture, PopulateCaps_Simple)
{
    const char *caplist = "CAP_NET_ADMIN,CAP_CHOWN";

    Json::Value root; 

    cap_value_t out[10];
    short count = 0;

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_NET_ADMIN"), _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(CAP_NET_ADMIN), Return(0)));

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_CHOWN"), _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(CAP_CHOWN), Return(0)));

    populate_capabilities(root, caplist, out, &count);

    EXPECT_EQ(count, 2);
    EXPECT_EQ(out[0], CAP_NET_ADMIN);
    EXPECT_EQ(out[1], CAP_CHOWN);
}

TEST_F(InitCapTestFixture, PopulateCaps_EmptyTokens)
{
    const char *caplist = ",CAP_NET_RAW,,CAP_CHOWN,";

    Json::Value root;
    cap_value_t out[10];
    short count = 0;

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_NET_RAW"), _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(CAP_NET_RAW), Return(0)));

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_CHOWN"), _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(CAP_CHOWN), Return(0)));

    populate_capabilities(root, caplist, out, &count);

    EXPECT_EQ(count, 2);
    EXPECT_EQ(out[0], CAP_NET_RAW);
    EXPECT_EQ(out[1], CAP_CHOWN);
}

TEST_F(InitCapTestFixture, PopulateCaps_InvalidName)
{
    const char *caplist = "CAP_FAKE";

    Json::Value root;
    cap_value_t out[10];
    short count = 0;

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_FAKE"), _))
        .Times(1)
        .WillOnce(Return(-1));   // simulate invalid

    populate_capabilities(root, caplist, out, &count);

    EXPECT_EQ(count, 0);
}

TEST_F(InitCapTestFixture, PopulateCaps_GroupRecursion)
{
    Json::Value root;
    root["group1"] = "CAP_DAC_OVERRIDE,CAP_NET_RAW";

    const char *caplist = "group1";

    cap_value_t out[10];
    short count = 0;

    // First attempt: treat "group1" as a capability
    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("group1"), _))
        .Times(1)
        .WillOnce(Return(-1));

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_DAC_OVERRIDE"), _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(CAP_DAC_OVERRIDE), Return(0)));

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_NET_RAW"), _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(CAP_NET_RAW), Return(0)));

    populate_capabilities(root, caplist, out, &count);

    EXPECT_EQ(count, 2);
    EXPECT_EQ(out[0], CAP_DAC_OVERRIDE);
    EXPECT_EQ(out[1], CAP_NET_RAW);
}

TEST_F(InitCapTestFixture, PopulateCaps_NestedGroups)
{
    Json::Value root;
    root["base"] = "CAP_CHOWN,CAP_NET_RAW";
    root["groupA"] = "base";

    const char *caplist = "groupA";

    cap_value_t out[10];
    short count = 0;

    // Fail first level recursion
    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("groupA"), _))
        .Times(1)
        .WillOnce(Return(-1));

    // Fail second level recursion
    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("base"), _))
        .Times(1)
        .WillOnce(Return(-1));

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_CHOWN"), _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(CAP_CHOWN), Return(0)));

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_NET_RAW"), _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(CAP_NET_RAW), Return(0)));

    populate_capabilities(root, caplist, out, &count);

    EXPECT_EQ(count, 2);
    EXPECT_EQ(out[0], CAP_CHOWN);
    EXPECT_EQ(out[1], CAP_NET_RAW);
}


TEST_F(InitCapTestFixture, PopulateCaps_Duplicate)
{
    const char *caplist = "CAP_NET_RAW,CAP_NET_RAW";

    Json::Value root;
    cap_value_t out[10];
    short count = 0;

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_NET_RAW"), _))
        .Times(2)   // called twice
        .WillRepeatedly(
            DoAll(SetArgPointee<1>(CAP_NET_RAW), Return(0))
        );

    populate_capabilities(root, caplist, out, &count);

    EXPECT_EQ(count, 2);
    EXPECT_EQ(out[0], CAP_NET_RAW);
    EXPECT_EQ(out[1], CAP_NET_RAW);
}

TEST_F(InitCapTestFixture, PopulateCaps_Invalid)
{
    Json::Value root;
    cap_value_t out[10];
    short count = 0;

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_FAKE"), _))
        .WillOnce(Return(-1));

    populate_capabilities(root, "CAP_FAKE", out, &count);

    EXPECT_EQ(count, 0);
}

TEST_F(InitCapTestFixture, PopulateCaps_InvalidGroupWithoutRecursion)
{
    Json::Value root;   
    cap_value_t out[10];
    short count = 0;

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("UNKNOWN"), _))
        .Times(1)
        .WillOnce(Return(-1));

    populate_capabilities(root, "UNKNOWN", out, &count);

    EXPECT_EQ(count, 0);
}

TEST_F(InitCapTestFixture, PopulateCaps_SingleToken)
{
    Json::Value root;
    cap_value_t out[10];
    short count = 0;

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("CAP_SYSLOG"), _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(CAP_SYSLOG), Return(0)));

    populate_capabilities(root, "CAP_SYSLOG", out, &count);

    EXPECT_EQ(count, 1);
    EXPECT_EQ(out[0], CAP_SYSLOG);
}

TEST_F(InitCapTestFixture, InitCapability_Success)
{
    cap_t fake_cap = (cap_t)0xdeadbeef;

    EXPECT_CALL(*g_CapMock, cap_get_proc())
        .Times(1)
        .WillOnce(Return(fake_cap));

    cap_t ret = init_capability();

    EXPECT_EQ(ret, fake_cap);
}

TEST_F(InitCapTestFixture, InitCapability_Failure)
{
    EXPECT_CALL(*g_CapMock, cap_get_proc())
        .Times(1)
        .WillOnce(Return((cap_t)NULL));

    cap_t ret = init_capability();

    EXPECT_EQ(ret, (cap_t)NULL);
}

TEST_F(InitCapTestFixture, DropRootCaps_AlreadyNonRoot)
{
    cap_user cu{};
    cu.user_name = strdup("dummy");
    cu.default_count = 0;
    cu.add_count = 0;
    cu.drop_count = 0;

    EXPECT_CALL(*g_CapMock, getpid())
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return((pid_t)123));

    EXPECT_CALL(*g_CapMock, getuid())
        .Times(1)
        .WillOnce(Return((uid_t)1000)); // non-root


    int rc = drop_root_caps(&cu);

    EXPECT_EQ(rc, 0);
}

TEST_F(InitCapTestFixture, DropRootCaps_NoUserName_NonRoot)
{
    cap_user cu{};
    cu.user_name = NULL;   // no username

    EXPECT_CALL(*g_CapMock, getpid())
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return((pid_t)999));

    // Non-root early return
    EXPECT_CALL(*g_CapMock, getuid())
        .Times(1)
        .WillOnce(Return((uid_t)1000));

    int rc = drop_root_caps(&cu);

    EXPECT_EQ(rc, 0);

    // IMPORTANT: user_name must remain NULL (not set)
    EXPECT_EQ(cu.user_name, nullptr);
}

TEST_F(InitCapTestFixture, UpdateProcCaps_SetProcFails)
{
    cap_user cu{};
    cu.add_count = 0;
    cu.drop_count = 0;

    // No flag operations expected
    EXPECT_CALL(*g_CapMock, cap_set_flag(_,_,_,_,_)).Times(::testing::AnyNumber());

    EXPECT_CALL(*g_CapMock, cap_set_proc(_))
        .WillOnce(Return(-1));  // FAIL

    int rc = update_process_caps(&cu);
    EXPECT_EQ(rc, -1);
}

TEST_F(InitCapTestFixture, ReadCaps_ExitOnCapGetPidNull)
{
    // arrange fake cap_user
    cap_user cu{};

    // Inside death test, nothing from mocks is reliable,
    // so force getpid() to return any value before fork-time.
    EXPECT_CALL(*g_CapMock, getpid())
        .WillOnce(Return((pid_t)111));

    // Do NOT set expectation on cap_get_pid()
    // because mocks do not survive the fork used by death tests.

    // Death test: just confirm the function exits
    EXPECT_DEATH(
        {
            // during forked child <E2><86><92> g_CapMock may be NULL
            read_capability(&cu);
        },
        ""
    );
}

TEST_F(InitCapTestFixture, GetProcessName_FopenFails)
{
    cap_user cu{};
    cu.user_name = strdup("non-root");

    // Simulate process ID pointing to nonexistent file
    EXPECT_CALL(*g_CapMock, getpid())
        .WillRepeatedly(Return(999999));  // No such /proc/999999/comm

    EXPECT_CALL(*g_CapMock, getuid())
        .WillOnce(Return((uid_t)1000)); // already non-root

    int rc = drop_root_caps(&cu);
    EXPECT_EQ(rc, 0);
}

TEST_F(InitCapTestFixture, UpdateProcessCaps_AddAndDrop)
{
    cap_user cu{};
    cu.add_count = 2;
    cu.drop_count = 1;

    // Local arrays
    cap_value_t adds[2] = { CAP_CHOWN, CAP_DAC_OVERRIDE };
    cap_value_t drops[1] = { CAP_NET_RAW };

    // Copy into struct arrays
    memcpy(cu.add, adds, sizeof(adds));
    memcpy(cu.drop, drops, sizeof(drops));

    EXPECT_CALL(*g_CapMock, cap_set_flag(_,_,_,_,_))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*g_CapMock, cap_set_proc(_))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*g_CapMock, cap_get_ambient(_))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*g_CapMock, cap_set_ambient(_, _))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*g_CapMock, getpid())
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return((pid_t)123));

    EXPECT_CALL(*g_CapMock, cap_free(_))
        .Times(::testing::AnyNumber())
        .WillRepeatedly(Return(0));

    int rc = update_process_caps(&cu);
    EXPECT_EQ(rc, 0);
}

TEST_F(InitCapTestFixture, SetAmbientCaps_FailurePath)
{
    cap_value_t caps[1] = { CAP_NET_ADMIN };

    EXPECT_CALL(*g_CapMock, cap_set_ambient(_, _))
        .WillOnce(Return(-1));

    EXPECT_CALL(*g_CapMock, cap_to_name(_))
        .WillOnce(Return(strdup("CAP_NET_ADMIN")));

    EXPECT_CALL(*g_CapMock, cap_free(_))
        .Times(1);

    set_ambient_caps(caps, 1, CAP_SET);
}

TEST_F(InitCapTestFixture, GainRootPrivilege_SetGidFails_Death)
{
    // Allow mock to respond in child process
    ON_CALL(*g_CapMock, setgid(0))
        .WillByDefault(Return(-1));

    ASSERT_DEATH(gain_root_privilege(), "");
}

TEST_F(InitCapTestFixture, GainRootPrivilege_SetUidFails_Death)
{
    ON_CALL(*g_CapMock, setgid(0))
        .WillByDefault(Return(0));

    ON_CALL(*g_CapMock, setuid(0))
        .WillByDefault(Return(-1));

    ASSERT_DEATH(gain_root_privilege(), "");
}

TEST_F(InitCapTestFixture, PopulateCaps_GroupResolvesEmpty)
{
    Json::Value root;
    root["groupX"] = "";   // empty group

    cap_value_t out[5];
    short count = 0;

    EXPECT_CALL(*g_CapMock, cap_from_name(StrEq("groupX"), _))
        .WillOnce(Return(-1));

    populate_capabilities(root, "groupX", out, &count);
    EXPECT_EQ(count, 0);
}

TEST_F(InitCapTestFixture, UpdateProcCaps_NoAddNoDrop)
{
    cap_user cu{};
    cu.add_count = 0;
    cu.drop_count = 0;

    EXPECT_CALL(*g_CapMock, cap_set_proc(_))
        .WillOnce(Return(0));

    EXPECT_EQ(update_process_caps(&cu), 0);
}

TEST_F(InitCapTestFixture, GetCapabilities_FullSuccess)
{
    int rc = system("mkdir -p /etc/security/caps");
    ASSERT_EQ(rc, 0);

    std::ofstream ofs("/etc/security/caps/process-capabilities.json");
    ASSERT_TRUE(ofs.is_open());

    ofs <<
    "{"
    "  \"default\": \"CAP_CHOWN\","
    "  \"myproc\": {"
    "    \"allow\": \"CAP_NET_ADMIN\","
    "    \"drop\": \"CAP_SYSLOG\""
    "  }"
    "}";

    ofs.close();

    cap_user cu{};
    get_capabilities("myproc", &cu);

    EXPECT_GT(cu.default_count, 0);
    EXPECT_GT(cu.add_count, 0);
    EXPECT_GT(cu.drop_count, 0);

    unlink("/etc/security/caps/process-capabilities.json");
}
