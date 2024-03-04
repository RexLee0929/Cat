document.getElementById('volunteerApplicationForm').addEventListener('submit', function(event) {
    event.preventDefault();

    const Onlyvolapp = 'V' + Date.now().toString();
    // 在提交表单前请求用户名和密码
    const username = prompt("请输入用户名:");
    const password = prompt("请输入密码:");

    // 收集表单数据
    const formData = {
        username: username,
        password: password,
        name: document.querySelector('input[name="name"]').value,
        gender: document.querySelector('select[name="gender"]').value,
        birthdate: document.querySelector('input[name="birthdate"]').value,
        city: document.querySelector('input[name="city"]').value,
        phone: document.querySelector('input[name="phone"]').value,
        reason: document.querySelector('textarea[name="reason"]').value,
        volapp_Only: Onlyvolapp
    };

    fetch('/api/submit_volunteer_application', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(formData)
    })
    .then(response => response.json())
    .then(data => {
        if (data.message) {
            // 显示成功消息和申请单唯一标识
            alert(data.message + ". 你的申请单号是: " + Onlyvolapp);
        } else {
            // 显示错误消息
            alert("申请提交失败: " + data.error);
        }
    })
    .catch(error => {
        console.error('Error:', error);
        alert('提交志愿者申请失败');
    });
});
