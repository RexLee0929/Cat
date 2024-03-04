document.getElementById('registerForm').addEventListener('submit', function(event) {
    event.preventDefault(); // 阻止表单的默认提交行为

    const formData = {
        username: document.querySelector('input[name="username"]').value,
        email: document.querySelector('input[name="email"]').value,
        password: document.querySelector('input[name="password"]').value,
        confirmPassword: document.querySelector('input[name="confirmPassword"]').value
    };

    // 简单的前端验证
    if(formData.password !== formData.confirmPassword) {
        alert('两次输入的密码不一致！');
        return;
    }

    fetch('/api/register', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(formData)
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('网络响应表示失败');
        }
        return response.json(); // 现在后端返回的是JSON格式的数据
    })
    .then(data => {
        console.log('Success:', data);
        alert(data.message); // 使用data.message来获取成功消息
        window.location.href = 'login.html';
    })
    .catch((error) => {
        console.error('Error:', error);
        alert('注册失败');
    });
    
});
