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
extern "C" {
#include "cap.h"
#include<string.h>
#include<unistd.h>
#include<sys/capability.h>
}

using  namespace std;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

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

/*TEST_F(InitCapTestFixture, DropRootPositive)
{
        int retval=-1;
        cap_t ret_cap =(cap_t) 0xffffffff;
        char test[]="non-root";
        cap_user appcaps;
appcaps.add_count = 0;
appcaps.default_count = 0;
appcaps.drop_count = 0;
        uid_t retid = 0;
        EXPECT_CALL(*g_CapMock, getuid())
                .Times(2)
                .WillOnce(Return(retid))
                .WillOnce(Return(retid));
        EXPECT_CALL(*g_CapMock, cap_clear_flag(_,_))
                .Times(2)
                .WillOnce(Return(0))
                .WillOnce(Return(0));
        EXPECT_CALL(*g_CapMock, cap_get_proc())
                .Times(1)
                .WillOnce(Return(ret_cap));
        EXPECT_CALL(*g_CapMock, cap_get_pid(_))
                .Times(1)
                .WillOnce(Return(ret_cap));

        EXPECT_CALL(*g_CapMock, cap_set_proc(_))
                .Times(2)
                .WillOnce(Return(0))
                .WillOnce(Return(0));
        EXPECT_CALL(*g_CapMock, cap_set_flag(_,_,_,_,_))
                .Times(3)
                .WillOnce(Return(0))
                .WillOnce(Return(0))
                .WillOnce(Return(0));
        EXPECT_CALL(*g_CapMock, cap_get_ambient(_))
                .Times(2)
                .WillOnce(Return(0))
                .WillOnce(Return(0));
        EXPECT_CALL(*g_CapMock, cap_to_text(_,_))
                .Times(1)
                .WillOnce(Return(test));
       EXPECT_CALL(*g_CapMock, cap_free(_))
                .Times(3)
                .WillOnce(Return(0))
                .WillOnce(Return(0))
                .WillOnce(Return(0));

        EXPECT_EQ(true, drop_root_priv(&appcaps));    
}
TEST_F(InitCapTestFixture, DropRootTest)
{
        cap_user appcaps;
appcaps.add_count = 1;
appcaps.default_count = 1;
        cap_t ret_cap =(cap_t) 0xffffffff;
uid_t retid = 0;
EXPECT_CALL(*g_CapMock, getuid())
                .Times(2)
                .WillOnce(Return(retid))
                .WillOnce(Return(retid));
        EXPECT_CALL(*g_CapMock, cap_get_proc())
                .Times(1)
                .WillOnce(Return(ret_cap));
        EXPECT_CALL(*g_CapMock, cap_set_proc(_))
                .Times(1)
                .WillOnce(Return(-1));
        EXPECT_CALL(*g_CapMock, cap_clear_flag(_,_))
                .Times(2)
                .WillOnce(Return(0))
                .WillOnce(Return(0));
       EXPECT_CALL(*g_CapMock, cap_set_flag(_,_,_,_,_))
                .Times(4)
                .WillOnce(Return(0))
                .WillOnce(Return(0))
                .WillOnce(Return(0))
                .WillOnce(Return(0));
        EXPECT_EQ(false, drop_root_priv(&appcaps));
}*/
