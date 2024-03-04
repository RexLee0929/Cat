document.addEventListener("DOMContentLoaded", function() {
    fetchTasks();
});

function fetchTasks() {
    fetch('/api/get_rescue_tasks')
    .then(response => {
        if (!response.ok) {
            throw new Error('网络响应错误');
        }
        return response.json();
    })
    .then(data => {
        const doingTasksDiv = document.getElementById("doing-tasks");
        const completedTasksDiv = document.getElementById("completed-tasks");

        doingTasksDiv.innerHTML = '';
        completedTasksDiv.innerHTML = '';

        data.tasks.forEach(task => {
            const taskHtml = `
                <div class="task">
                    <h3>${task.animal_info} (${task.animal_count}只)</h3>
                    <p>状态: ${task.animal_status}</p>
                    <p>地址: ${task.rescue_address}</p>
                    <p>申请人: ${task.applicant_name} (${task.applicant_gender}, ${task.applicant_city}, ${task.applicant_phone})</p>
                    <p>证明图片: <a href="${task.proof_image}" target="_blank">查看</a></p>
                    <p>证明视频: <a href="${task.proof_video}" target="_blank">查看</a></p>
                    <p>指派志愿者: ${task.assigned_volunteer || '未指派'}</p>
                    ${!task.is_finished ? `<button class="apply-rescue-btn" data-rescue-id="${task.id}">申请加入救助</button>` : ''}
                </div>
            `;

            if (task.is_finished) {
                completedTasksDiv.innerHTML += taskHtml;
            } else {
                doingTasksDiv.innerHTML += taskHtml;
            }
        });

        attachButtonListeners();
    })
    .catch(error => {
        console.error('错误:', error);
    });
}

function attachButtonListeners() {
    document.querySelectorAll('.apply-rescue-btn').forEach(button => {
        button.addEventListener('click', function(event) {
            applyForRescue();
        });
    });
}

function applyForRescue() {
    const rescueOnly = 'R' + Date.now(); // 前端生成rescue_only
    const username = prompt("请输入志愿者用户名:");
    const password = prompt("请输入密码:");

    fetch('/api/volunteer_apply_rescue', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            username: username,
            password: password,
            rescueOnly: rescueOnly // 添加rescueOnly字段
        })
    })
    .then(response => {
        if (!response.ok) {
            // 当响应不是OK时，将响应JSON转换为对象以获取错误消息
            return response.json().then(err => {
                throw new Error(err.error || '未知错误'); // 使用服务器返回的错误消息，或者一个通用的未知错误消息
            });
        }
        return response.json();
    })
    .then(data => {
        alert(data.message + "\n申请单号: " + rescueOnly); // 展示申请单号给用户
        // 刷新任务列表以显示最新状态
        fetchTasks();
    })
    .catch(error => {
        console.error('错误:', error);
        alert(error.message); // 显示更具体的错误消息给用户
    });
}
