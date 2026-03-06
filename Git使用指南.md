# Git使用指南 - 麻将游戏项目

## Git配置已完成

你的Git已经配置好：
- **用户名**: Mr.L
- **邮箱**: 546607861@qq.com
- **项目位置**: `/home/lihao/.openclaw/workspace/mahjong-c-project/`

## 当前Git状态

### 提交历史
```
b5f8e1c 添加完整的服务器实现
e7c3a9d 项目初始化：C语言麻将游戏完整框架
d41c8e2 初始提交：添加.gitignore文件
```

### 查看状态
```bash
cd /home/lihao/.openclaw/workspace/mahjong-c-project
git status
```

## 基础Git命令

### 1. 查看状态和修改
```bash
# 查看当前状态
git status

# 查看具体修改内容
git diff

# 查看提交历史
git log --oneline
git log --graph --oneline  # 图形化显示
```

### 2. 添加和提交
```bash
# 添加单个文件
git add 文件名.c

# 添加所有修改的文件
git add .

# 提交到本地仓库
git commit -m "描述修改内容"

# 修改上次提交（如果忘记添加文件）
git add 忘记的文件.c
git commit --amend -m "更新提交信息"
```

### 3. 分支管理
```bash
# 查看分支
git branch

# 创建新分支
git branch 新分支名

# 切换到分支
git checkout 分支名

# 创建并切换到新分支
git checkout -b 新分支名

# 合并分支
git checkout main
git merge 分支名

# 删除分支
git branch -d 分支名
```

## 项目开发工作流

### 日常开发流程
```bash
# 1. 开始新功能前，创建分支
git checkout -b feature/新功能名称

# 2. 编写代码，经常提交
git add .
git commit -m "实现xxx功能"

# 3. 完成功能后，合并到主分支
git checkout main
git merge feature/新功能名称

# 4. 删除功能分支
git branch -d feature/新功能名称
```

### 修复Bug流程
```bash
# 1. 从主分支创建修复分支
git checkout -b fix/修复的问题描述

# 2. 修复并提交
git add .
git commit -m "修复xxx问题"

# 3. 测试通过后合并
git checkout main
git merge fix/修复的问题描述
```

## 实用技巧

### 1. 撤销修改
```bash
# 撤销工作目录的修改（危险！会丢失修改）
git checkout -- 文件名.c

# 撤销暂存区的修改（保留工作目录修改）
git reset HEAD 文件名.c

# 撤销最近一次提交（创建新的撤销提交）
git revert HEAD
```

### 2. 查看历史
```bash
# 查看某个文件的修改历史
git log --oneline 文件名.c

# 查看某次提交的具体修改
git show 提交ID

# 查看谁修改了哪行代码
git blame 文件名.c
```

### 3. 暂存修改
```bash
# 临时保存当前修改（切换分支时有用）
git stash

# 恢复暂存的修改
git stash pop

# 查看所有暂存
git stash list
```

## 项目特定命令

### 编译和测试
```bash
# 编译项目
make all

# 运行测试
make test

# 运行服务器
make run

# 清理编译文件
make clean
```

### 开发检查清单
```bash
# 1. 检查代码是否能编译
make all

# 2. 运行测试
make test

# 3. 检查Git状态
git status

# 4. 提交代码
git add .
git commit -m "描述修改"

# 5. 推送到远程仓库（如果有）
git push origin main
```

## 常见问题

### 问题1：提交时忘记配置用户信息
```bash
git config --global user.name "Mr.L"
git config --global user.email "546607861@qq.com"
```

### 问题2：提交了错误的文件
```bash
# 方法1：修改上次提交
git add 正确的文件.c
git commit --amend -m "修正提交"

# 方法2：创建新的提交来修正
git add 修正的文件.c
git commit -m "修正之前的错误"
```

### 问题3：想回到之前的版本
```bash
# 查看历史版本
git log --oneline

# 临时回到某个版本（查看）
git checkout 提交ID

# 永久回到某个版本（危险！会丢失之后的提交）
git reset --hard 提交ID
```

## 学习资源

### 交互式教程
- https://learngitbranching.js.org/ (推荐！图形化学习)
- https://try.github.io/ (GitHub官方教程)

### 文档和书籍
- Git官方文档：https://git-scm.com/doc
- 廖雪峰Git教程：https://www.liaoxuefeng.com/wiki/896043488029600

### 可视化工具
- GitKraken (图形化Git客户端)
- SourceTree (免费的Git GUI)
- VSCode内置的Git功能

## 下一步

### 今天可以练习的
1. 创建一个新分支 `git checkout -b practice`
2. 修改 `src/common/tile.c` 文件
3. 提交修改 `git add . && git commit -m "练习修改"`
4. 切换回主分支 `git checkout main`
5. 查看分支差异 `git diff practice`

### 项目开发
1. 在 `develop` 分支上开发新功能
2. 使用 `feature/` 前缀命名功能分支
3. 每次完成一个小功能就提交
4. 定期运行测试确保代码质量

记住：**小步快跑，频繁提交**。不要等到写完一大堆代码才提交，每完成一个小功能就提交一次。